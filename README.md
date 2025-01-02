# A simple tool for generating fractals

### ContinuousFractals config syntax:
```
{
  "globals": {
    "IMAGE_HEIGHT": 512,
    "IMAGE_WIDTH": 512,
    "OUTPUT_DIR": "C:\\Users\\Sheri\\Documents\\ContinuousFractals\\Output"
  },
  "fractals": [
    {
        "name": "MandelBox",
        "formulaScale": 2,
        "iterationCount": 300,
        "colourizer": "BlueShades",
        "minDepth": -0.9,
        "maxDepth": 1,
        "increment": 0.01,
        "disable" : true
    },
    {		
        "name": "JuliaSet",
        "functionName" : "donut",
        "scalarMin" : 
        {
            "r" : -0.18,
            "i" : -0.18
        },
        "scalarMax" : 
        {
            "r" : 0.18,
            "i" : 0.18
        },		
        "scalarIncrement" :
        {
            "r" : 0.005,
            "i" : 0.005			
        },
        "formulaScale": 2.1,
        "iterationCount": 300,
        "colourizer": "OutputScale",
        "colour" : "#000000",
        "minDepth": -0.9,
        "maxDepth": 1,
        "increment": 0.1
    }
  ]
}
```
---
### Global Options
| Value | Type | Description | Possible values |
| ------------- | --- |-------------| -----|
| IMAGE_HEIGHT | _int_ | Generated image height in pixels | Any positive int |
| IMAGE_WIDTH  | _int_ | Generated image width in pixels | Any positive int |
| OUTPUT_DIR | _string_ | Output path for generated images |
---
Global options should be followed by a list of fractals to generate
### Fractal options
| Value | Type | Description | Possible values |
| ------------- | --- |-------------| -----|
| name      | _string_ | The kind of fractal you want to generate | `"MandleBox"`, `"JuliaSet"` |
| formulaScale      | _float_ | The scale of the formula used in the MandleBox generation | Any float |
| iterationCount | _int_ | The number of iterations to perform when determing if a point is in the fractal set | Any positive int |
| colourizer | _string_ | The type of colourizer to use when colouring the fractal image output | `"OutputScaled"`, `"BlueShades"`, `"B&W"`, `"TimeRainbow"` |
| colour | _string_ | The hex value for a colour to be used in a colourizer. If no colourizer is specified the fractal will be coloured using this colour | Any hex colour (preceded with #) |
| disable | _bool_ | Flag to disable fractal generation |
---
See more generated output here: https://github.com/KatamariDad/ContinuousFractals/tree/master/CoolOutput

![image](https://github.com/user-attachments/assets/6ab925ff-1def-4e07-9af5-38eef2bdb3a2)

![image](https://github.com/user-attachments/assets/ca67d146-c3a4-4c07-969c-ec4c2103fbb0)

![image](https://github.com/user-attachments/assets/f6558b47-8088-45fa-99bc-8e810f311538)
---
### Future Dev:
3D fractal ray traced visualization
GUI

