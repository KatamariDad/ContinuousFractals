#pragma once
#include <cstdint>
#include <string>

#include <Math/Vector3.h>

struct PixelColour;
class FractalColourizer;
class FractalFunctor3D;
class Vector3f;

namespace Image
{
	class Image;
}

class FractalGenerator3D
{
public:

	class GenerateParams
	{
	public:
		GenerateParams(const FractalColourizer& _colourizer);
		~GenerateParams();

		Vector3f scale;
		Vector3f origin;
		const FractalColourizer& colourizer; 
		bool multithreadEnabled;
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

class FractalColourizer
{
public:
	virtual void GenerateColour(
		const Vector3f& input,
		const Vector3f& functionOutput,
		const bool isInSet,
		const uint32_t divergenceIteration,
		PixelColour& outColour ) const = 0;
};


class FractalFunctor3D
{
public:
	FractalFunctor3D() = default;

    // given a complex number,
    // generate a colour.
	virtual void GenerateColourForInput(
		const Vector3f& input,
		const FractalColourizer& colourizer,
		PixelColour& outColour ) const = 0;

	virtual std::string GetParamDesc() const = 0;
};
