#pragma once
#include <Fractal/Colourizers/Colourizers.h>
#include <ctime>


class ColourSchemeTime final : public FractalColourizer
{
public:
	ColourSchemeTime();
	virtual void GenerateColour(
		const Vector3f& input,
		const Vector3f& functionOutput,
		const bool isInSet,
		const uint32_t divergenceIteration,
		PixelColour& outColour ) const override;

	virtual const char* ToString() const;

private:
	tm m_now;
	int32_t m_currentHour;
	int32_t m_currentMinute;
};

