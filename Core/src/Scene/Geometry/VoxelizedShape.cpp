#include "pch.h"
#include "VoxelizedShape.h"
#include "Math/Math.h"
#include <fstream>
#include <string>

VoxelizedShape::VoxelizedShape(const std::string& voxFilename)
{
	std::ifstream fileIn(voxFilename,std::ios::in);
	Deserialize(fileIn);
}

bool VoxelizedShape::IntersectRay(
	const Vector3f& sceneOrigin,
	const Vector3f& rayOrigin,
	const Vector3f& rayDir,
	Vector3f& hitLocation,
	Vector3f& hitNormal) const
{
	const Vector3f VoxelHalfExtent = (0.5f * m_voxelExtent);

	// minor optimization to do a quick test on the shape's AABB
	const Vector3f size(m_voxelData.size(), m_voxelData[0].size(), m_voxelData[0][0].size());
	const Vector3f BoundsMin = sceneOrigin - (size * VoxelHalfExtent) - Vector3f(0.01f);
	const Vector3f BoundsMax = sceneOrigin + (size * VoxelHalfExtent) + Vector3f(0.01f);

	Vector3f LocalHit, LocalNormal;
	if (!AABBIntersectRay(sceneOrigin, BoundsMax - BoundsMin, rayOrigin, rayDir, LocalHit, LocalNormal))
	{
		return false;
	}

	bool bHasHit = false;
	float ClosestHitDist = std::numeric_limits<float>::max();


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
						const float LocalHitDistToCamera = (LocalHit - rayOrigin ).SquaredLength();
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

	return bHasHit;
}


void VoxelizedShape::Serialize(std::ofstream& outStream)
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

}
