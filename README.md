## Pinhole Camera
This is a project for a dual credit engineering class.
It is not open for contribution at this moment, however you are welcome to try to use the calculators.

These are designed for shoebox cameras to calculate the distance you need to stand from the object and the amount of exposure time required in order to take a good picture based on a few factors, such as:
- Focal length
- Aperture size
- Film width/height
- Object width/height

The calculators will find exposure time and distance based on these inputs, and show you the math for figuring it out.

These calculators are compiled and build for Unix operating systems using the ELF format. You can download these beniaries from the `release` section on the github page. If you wish to use them on a Windows operating system, you will need to clone the source code with ```git clone https://github.com/Duckless137/pinhole_camera.git``` and recompile it using Mingw32-make. 
## How to use
1. After adding the binaries to your current working directory or `path`, you will need to open a new shell in any directory of your choosing.
2. In your current working directory, create a new file called `pinhole.conf`. You may choose a different name, but you will need to pass in the path to the config file to the calculator CLI.
It should be formatted like so:
```
# Example pinhole.conf for distance calculator
FOCAL_LEN = 31cm
OBJECT_DIMENSION = 108cm
FILM_DIMENSION = 18cm
```
- Field names must be all caps (with underscores)
- Units must be all lowercase
- Values must be numbers with decimals 
- All whitespace is ignored

The calculator will let you know what fields you need - just run it with an empty `pinhole.conf` file and it will tell you what's missing.

## Distance Calculator
Find the dimensions of the object you want to take a photo of and the dimensions of your film. Choose either width or height and get your choice of both the object and the film and plug that into the calculator.
For example, with a `(180cm by 120cm)` object and `(18cm by 12cm)` film you will want to plug in either `180cm & 18cm` or `120cm and 12cm`.

Required fields:
```
FOCAL_LEN
OBJECT_DIMENSION
FILM_DIMENSION
```
- FOCAL_LEN: Camera's focal length
- FILM_DIMENSION: The film's width or height.
- OBJECT_DIMENSION: The object's width or height. If you used the film's height before, use the object's height, and vise versa.

Remember, all units provided must be the same!

## Duration Calculator
First, take a picture of your aperture hole with a ruler. Load that into Photoshop, GIMP, MS Paint, or some other form of image manipulation program. Use said program to find the width of the aperture hole in pixels and use the ruler in the picture to find out how many pixels are in the unit of your choice. Now, plug the values into the calculator.

Required fields:
```
FOCAL_LEN
PIX_SPAN
PIX_PER_UNIT
```
- FOCAL_LEN: Camera's focal length
- PIX_SPAN: The aperture's width in pixels
- PIX_PER_UNIT: Howver many pixels there are per the unit used in FOCAL_LEN

## Contributing
What? No. No contributions.
