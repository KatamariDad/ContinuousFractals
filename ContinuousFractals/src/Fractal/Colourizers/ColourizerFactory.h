#pragma once
#include <Factory/Factory.h>

#include "Colourizers.h"
#include "TimeRainbow.h"

template<>
class Factory<FractalColourizer>
{
public:
	std::unique_ptr<FractalColourizer> Build( const std::string& key, const nlohmann::json& params )
	{
		if (key == "OutputScale")
		{
			return std::unique_ptr<FractalColourizer>( new SimpleColourScaledByFunctorOutputValue() );
		}
		else if (key == "BlueShades")
		{
			return std::unique_ptr<FractalColourizer>( new ShadesOfBlueColourizer() );
		}
		else if (key == "TimeRainbow")
		{
			return std::unique_ptr<FractalColourizer>( new TimeRainbow() );
		}
		else if (key == "Solid")
		{
			std::string hexCode = params["colour"];
			return std::unique_ptr<FractalColourizer>( new SolidColour( PixelColour( hexCode ) ) );
		}
		else
		{
			return std::unique_ptr<FractalColourizer>( new BlackAndWhite() );
		}
	}
};

