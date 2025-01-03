#pragma once
#include <Scene/Geometry/Geometry.h>
#include <vector>

class VoxelizedShape final : public Geometry
{
public:
	VoxelizedShape() {}

	VoxelizedShape(const std::string& voxFilename);

	VoxelizedShape(
		const Vector3f& voxelExtent, 
		const std::vector<std::vector<std::vector<bool>>>& voxelData)
		: m_voxelExtent(voxelExtent)
		, m_voxelData(voxelData)
	{}

	static std::string GetJsonTypeName() { return "vox"; }

	virtual bool IntersectRay(
		const Vector3f& sceneOrigin,
		const Vector3f& rayOrigin,
		const Vector3f& rayDir,
		Vector3f& hitLocation,
		Vector3f& hitNormal) const override;

	void Serialize(std::ofstream& outStream);
	void Deserialize(std::ifstream& inStream);

private:

	std::vector<std::vector<std::vector<bool>>> m_voxelData;
	Vector3f m_voxelExtent;

};

