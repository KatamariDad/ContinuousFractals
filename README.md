# A simple tool for generating fractals

### ContinuousFractals config syntax:
```
{
  "fractals": [
    {
      "name": "MandelBox",
      "formulaScale": 2,
      "iterationCount": 300,
      "colourizer": "BlueShades"
    },
    {
      "name": "MandelBox",
      "formulaScale": 2,
      "iterationCount": 300,
      "colourizer": "BlueShades",
      "colour": "#ff0000"
    }
  ]
}
```
---
| Value | Type | Description | Possible values |
| ------------- | --- |-------------| -----|
| name      | _string_ | The kind of fractal you want to generate | `"MandleBox"` |
| formulaScale      | _float_ | The scale of the formula used in the MandleBox generation | Any float |
| iterationCount | _int_ | The number of iterations to perform when determing if a point is in the fractal set | Any int |
| colourizer | _string_ | The type of colourizer to use when colouring the fractal image output | `"OutputScaled"`, `"BlueShades"`, `"B&W"`, `"TimeRainbow"` |
| colour | _string_ | The hex value for a colour to be used in a colourizer. If no colourizer is specified the fractal will be coloured using this colour | Any hex colour (preceded with #) |

