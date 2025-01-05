#pragma once
#include <cstdint>
#include <string>

#include <Math/Vector3.h>
#include <Math/ComplexNumber.h>

#include <JSON/json.hpp>

struct PixelColour;
class FractalColourizer;
class FractalFunctor3D;
class FractalFunctor2DComplex;
class Vector3f;

namespace Image
{
	class Image;
}

class FractalGenerator
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
		uint32_t depthIndex = 0;
		bool multithreadEnabled;
	};

	void Generate(
		Image::Image& outImage,
		const GenerateParams& params,
		const FractalFunctor3D& fractalFunctor,
		class VoxelizedShape* voxelizedShape );

	void Generate(
		Image::Image& outImage,
		const GenerateParams& params,
		const FractalFunctor2DComplex& fractalFunctor );

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

	virtual const char* ToString() const = 0;
};


class FractalFunctor3D
{
public:
	FractalFunctor3D() = default;

	virtual void GenerateColourForInput(
		const Vector3f& input,
		const FractalColourizer& colourizer,
		PixelColour& outColour,
		bool& bOutIsInSet ) const = 0;

	virtual std::string GetFractalDesc() const = 0;
};


class FractalFunctor2DComplex
{
public:
	FractalFunctor2DComplex() = default;

	virtual void GenerateColourForInput(
		const ComplexNumber& input,
		const FractalColourizer& colourizer,
		PixelColour& outColour ) const = 0;

	virtual std::string GetFractalDesc() const = 0;
};
