#pragma once
#include <Fractal/Colourizers/Colourizers.h>
#include <ctime>


class TimeRainbow final : public FractalColourizer
{
public:
	TimeRainbow();
	virtual void GenerateColour(
		const Vector3f& input,
		const Vector3f& functionOutput,
		const bool isInSet,
		const uint32_t divergenceIteration,
		PixelColour& outColour ) const override;

private:
	time_t now;
};
