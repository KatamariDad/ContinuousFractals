#pragma once
#include <Scene/Geometry/Geometry.h>
#include <vector>

class VoxelizedShape final : public Geometry
{
public:
	VoxelizedShape() {}

	VoxelizedShape(const std::string& voxFilename);
	VoxelizedShape(uint32_t sizeX, uint32_t sizeY, uint32_t sizeZ, const Vector3f& voxelExtent);
	VoxelizedShape(const Vector3f& voxelExtent, const std::vector<std::vector<std::vector<bool>>>& voxelData);

	static std::string GetJsonTypeName() { return "vox"; }
	void Set(uint32_t x, uint32_t y, uint32_t z, bool val);

	virtual bool IntersectRay(
		const Vector3f& sceneOrigin,
		const Vector3f& rayOrigin,
		const Vector3f& rayDir,
		Vector3f& hitLocation,
		Vector3f& hitNormal) const override;

	void Save(const std::string& directory) const;
	void Serialize(std::ofstream& outStream) const;
	void Deserialize(std::ifstream& inStream);

private:

	void ComputeRadius();
	bool PointToIndices(const Vector3f& point, const Vector3f& origin, int32_t& x, int32_t& y, int32_t& z) const;

	std::vector<std::vector<std::vector<bool>>> m_voxelData;
	Vector3f m_voxelExtent;
	Vector3f m_size;
	float m_radiusSqr = 1.f;

};

