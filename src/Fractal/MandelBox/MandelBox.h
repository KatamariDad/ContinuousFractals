#pragma once
#include <Fractal/Fractal.h>
#include <Utility/Math/Vector3.h>


class MandelBox : public FractalFunctor3D
{
public:
	MandelBox(
		  float formulaScale
		, uint32_t maxIterations )
		: m_maxIterations( maxIterations )
		, m_formulaScale( formulaScale )
	{}


	virtual void GenerateColourForInput(
		const Vector3f& Input,
		uint8_t& r,
		uint8_t& g,
		uint8_t& b ) const override;

	virtual std::string GetParamDesc() const override;

private:

	void BallFold( Vector3f& input ) const;
	void BoxFold( float& input ) const;

	uint32_t m_maxIterations;
	float m_formulaScale;
};
