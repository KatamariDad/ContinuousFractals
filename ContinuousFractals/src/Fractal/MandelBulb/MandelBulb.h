#pragma once
#include <Fractal/Fractal.h>


class MandelBulb final : public FractalFunctor3D 
{
public:
	MandelBulb(
		  float exponent 
		, uint32_t maxIterations )
		: m_maxIterations( maxIterations )
		, m_order( exponent )
	{}

	void ResetParams(
		float formulaScale,
		uint32_t maxIterations )
	{
		m_order = formulaScale;
		m_maxIterations = maxIterations;
	}

	virtual void GenerateColourForInput(
		const Vector3f& Input,
		const FractalColourizer& colourizer,
		PixelColour& outColour,
		bool& bOutIsInSet ) const override;

	virtual std::string GetFractalDesc() const override;

private:


	uint32_t m_maxIterations;
	float m_order;
};

