#pragma once
#include <Fractal/Fractal.h>
#include <Math/Vector3.h>


class MandelBox final : public FractalFunctor3D 
{
public:
	MandelBox(
		  float formulaScale
		, uint32_t maxIterations )
		: m_maxIterations( maxIterations )
		, m_formulaScale( formulaScale )
	{}

	void ResetParams(
		float formulaScale,
		uint32_t maxIterations )
	{
		m_formulaScale = formulaScale;
		m_maxIterations = maxIterations;
	}

	virtual void GenerateColourForInput(
		const Vector3f& Input,
		const FractalColourizer& colourizer,
		PixelColour& outColour ) const override;

	virtual std::string GetParamDesc() const override;

private:

	void BallFold( Vector3f& input ) const;
	void BoxFold( float& input ) const;

	uint32_t m_maxIterations;
	float m_formulaScale;
};
