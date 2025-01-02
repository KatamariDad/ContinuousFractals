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

See more generated output here: https://github.com/KatamariDad/ContinuousFractals/tree/master/CoolOutput

![image](https://github.com/user-attachments/assets/6ab925ff-1def-4e07-9af5-38eef2bdb3a2)

![image](https://github.com/user-attachments/assets/ca67d146-c3a4-4c07-969c-ec4c2103fbb0)

![image](https://github.com/user-attachments/assets/f6558b47-8088-45fa-99bc-8e810f311538)

### Future Dev:
3D fractal ray traced visualization


