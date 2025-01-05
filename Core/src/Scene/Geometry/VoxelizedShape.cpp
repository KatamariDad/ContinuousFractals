#include "pch.h"

#include <fstream>
#include <limits>

#include "VoxelizedShape.h"
#include "Math/Math.h"
#include "IOManip/FileSystem.h"
#include "Time/TimeUtil.h"

VoxelizedShape::VoxelizedShape(
	const Vector3f& voxelExtent, 
	const std::vector<std::vector<std::vector<bool>>>& voxelData)
	: m_voxelExtent(voxelExtent)
	, m_voxelData(voxelData)
{
	ComputeRadius();
}

VoxelizedShape::VoxelizedShape(const std::string& voxFilename)
{
	std::ifstream fileIn(voxFilename,std::ios::in);
	Deserialize(fileIn);
}

VoxelizedShape::VoxelizedShape(uint32_t sizeX, uint32_t sizeY, uint32_t sizeZ, const Vector3f& voxelExtent)
{
	m_voxelExtent = voxelExtent;
	m_voxelData.resize(sizeX);

	for (int x = 0; x < m_voxelData.size(); ++x)
	{
		m_voxelData[x].resize(sizeY);
		for (int y = 0; y < m_voxelData[x].size(); ++y)
		{
			m_voxelData[x][y].resize(sizeZ);
			for (int z = 0; z < m_voxelData[x][y].size(); ++z)
			{
				m_voxelData[x][y][z] = false;
			}
		}
	}

	ComputeRadius();
}

void VoxelizedShape::Set(uint32_t x, uint32_t y, uint32_t z, bool val)
{
	m_voxelData[x][y][z] = val;
}

void VoxelizedShape::ComputeRadius()
{
	m_size = Vector3f(m_voxelData.size(), m_voxelData[0].size(), m_voxelData[0][0].size());
}

bool VoxelizedShape::IntersectRay(
	const Vector3f& sceneOrigin,
	const Vector3f& rayOrigin,
	const Vector3f& rayDir,
	Vector3f& hitLocation,
	Vector3f& hitNormal) const
{
	// minor optimization to do a quick test on the shape's AABB
	const Vector3f VoxelHalfExtent = (0.5f * m_voxelExtent);
	const Vector3f BoundsMin = sceneOrigin - (m_size * VoxelHalfExtent);
	const Vector3f BoundsMax = sceneOrigin + (m_size * VoxelHalfExtent);
	if (!AABBIntersectRay(sceneOrigin, 0.5f * (BoundsMax - BoundsMin), rayOrigin, rayDir, hitLocation, hitNormal))
	{
		return false;
	}


	bool bHasHit = false;
	float ClosestHitDist = std::numeric_limits<float>::max();

	bool bSlow = false;
	if (bSlow)
	{
		for (int x = 0; x < m_voxelData.size(); ++x)
		{
			for (int y = 0; y < m_voxelData[x].size(); ++y)
			{
				for (int z = 0; z < m_voxelData[x][y].size(); ++z)
				{
					if (m_voxelData[x][y][z])
					{
						Vector3f LocalHit, LocalNormal, voxelCenter;
						voxelCenter = BoundsMin + m_voxelExtent * Vector3f(x, y, z) + VoxelHalfExtent;
						if (AABBIntersectRay(voxelCenter, VoxelHalfExtent, rayOrigin, rayDir, LocalHit, LocalNormal))
						{
							const float LocalHitDistToCamera = (LocalHit - rayOrigin).SquaredLength();
							if (LocalHitDistToCamera < ClosestHitDist)
							{
								ClosestHitDist = LocalHitDistToCamera;
								hitLocation = LocalHit;
								hitNormal = LocalNormal;
								bHasHit = true;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		// Faster implementation :
		// Take the hit location on the outer AABB, and march along the ray from there.
		// Map each position to a voxel index, and only test for voxel AABB overlap when the index changes
		// Stop when the voxel indices are not valid.
		// Saw really good results compared to above when rendering a 64x64 image of a voxelized shape with about 100 voxels.
		// Execution time went from about 56 seconds to 2 seconds.
		const Vector3f BoundsMax = sceneOrigin + (m_size * VoxelHalfExtent);
		const Vector3f delta = 0.1f * VoxelHalfExtent;
		Vector3f testPoint = hitLocation + rayDir * delta;
		int32_t x = -1, y = -1, z = -1, newX = 0, newY = 0, newZ = 0;
		while (testPoint < BoundsMax)
		{
			if (PointToIndices(testPoint, sceneOrigin, newX, newY, newZ))
			{
				if (x != newX || y != newY || z != newZ)
				{
					x = newX; y = newY; z = newZ;

					if (m_voxelData[x][y][z])
					{
						Vector3f LocalHit, LocalNormal, voxelCenter;
						voxelCenter = BoundsMin + m_voxelExtent * Vector3f(x, y, z) + VoxelHalfExtent;
						if (AABBIntersectRay(voxelCenter, VoxelHalfExtent, testPoint, rayDir, LocalHit, LocalNormal))
						{
							const float LocalHitDistToCamera = (LocalHit - rayOrigin).SquaredLength();
							if (LocalHitDistToCamera < ClosestHitDist)
							{
								ClosestHitDist = LocalHitDistToCamera;
								hitLocation = LocalHit;
								hitNormal = LocalNormal;
								bHasHit = true;
								return true;
							}
						}
					}
				}
			}
			else
			{
				return false;
			}

			testPoint = testPoint + rayDir * delta;
		}
	}

	return bHasHit;
}

bool VoxelizedShape::PointToIndices(const Vector3f& point, const Vector3f& origin, int32_t& x, int32_t& y, int32_t& z) const
{
	const Vector3f VoxelHalfExtent = (0.5f * m_voxelExtent);
	const Vector3f BoundsMin = origin - (m_size * VoxelHalfExtent);

	const Vector3f diff = point - BoundsMin;
	x = std::floor(diff.x / m_voxelExtent.x);
	y = std::floor(diff.y / m_voxelExtent.y);
	z = std::floor(diff.z / m_voxelExtent.z);

	return x >= 0 && y >= 0 && z >= 0 && x < m_size.x&& y < m_size.y&& z < m_size.z;
}

void VoxelizedShape::Save(const std::string& directory) const
{
	FileSystem::MakeDirectory( directory );
	std::string filename = directory + "\\" + Time::CurrentTimeAndDate() + ".vox";
	std::ofstream out(filename, std::ios::binary);
	Serialize(out);
	out.close();
}

void VoxelizedShape::Serialize(std::ofstream& outStream) const
{
	outStream.write(reinterpret_cast<const char*>(&m_voxelExtent.x), sizeof(float));
	outStream.write(reinterpret_cast<const char*>(&m_voxelExtent.y), sizeof(float));
	outStream.write(reinterpret_cast<const char*>(&m_voxelExtent.z), sizeof(float));

	int sizeX = m_voxelData.size(), sizeY = m_voxelData[0].size(), sizeZ = m_voxelData[0][0].size();
	outStream.write(reinterpret_cast<const char*>(&sizeX), sizeof(int));
	outStream.write(reinterpret_cast<const char*>(&sizeY), sizeof(int));
	outStream.write(reinterpret_cast<const char*>(&sizeZ), sizeof(int));

	for (int x = 0; x < m_voxelData.size(); ++x)
	{
		for (int y = 0; y < m_voxelData[x].size(); ++y)
		{
			for (int z = 0; z < m_voxelData[x][y].size(); ++z)
			{
				const bool b = m_voxelData[x][y][z];
				outStream.write(reinterpret_cast<const char*>(&b), sizeof(bool));
			}
		}
	}
}

void VoxelizedShape::Deserialize(std::ifstream& inStream)
{
	inStream.read(reinterpret_cast<char*>(&m_voxelExtent.x), sizeof(float));
	inStream.read(reinterpret_cast<char*>(&m_voxelExtent.y), sizeof(float));
	inStream.read(reinterpret_cast<char*>(&m_voxelExtent.z), sizeof(float));

	int sizeX, sizeY, sizeZ;
	inStream.read(reinterpret_cast<char*>(&sizeX), sizeof(int));
	inStream.read(reinterpret_cast<char*>(&sizeY), sizeof(int));
	inStream.read(reinterpret_cast<char*>(&sizeZ), sizeof(int));

	m_voxelData.resize(sizeX);

	for (int x = 0; x < m_voxelData.size(); ++x)
	{
		m_voxelData[x].resize(sizeY);
		for (int y = 0; y < m_voxelData[x].size(); ++y)
		{
			m_voxelData[x][y].resize(sizeZ);
			for (int z = 0; z < m_voxelData[x][y].size(); ++z)
			{
				bool b = false;
				inStream.read(reinterpret_cast<char*>(&b), sizeof(bool));
				m_voxelData[x][y][z] = b;
			}
		}
	}

	ComputeRadius();
}
