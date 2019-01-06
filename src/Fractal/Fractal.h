#pragma once
#include <cstdint>
#include <string>

#include <Utility/Math/Vector3.h>

class FractalFunctor3D;
class Vector3f;

namespace Image
{
	class Image;
}

class FractalGenerator3D
{
public:

	struct GenerateParams
	{
		Vector3f scale;
		Vector3f Origin;
	};

    void Generate(
		Image::Image& outImage,
		const GenerateParams& params,
        const FractalFunctor3D& fractalFunctor
    );

    void AddXRotation(const float theta);
    void AddYRotation(const float theta);
    void AddZRotation(const float theta);
private:

    float m_rotateX;
    float m_rotateY;
    float m_rotateZ;
};


class FractalFunctor3D
{
public:
	FractalFunctor3D() = default;

    // given a complex number,
    // generate a colour.
	virtual void GenerateColourForInput(
		const Vector3f& Input,
		uint8_t& r,
		uint8_t& g,
		uint8_t& b ) const = 0;

	virtual std::string GetParamDesc() const = 0;
};
