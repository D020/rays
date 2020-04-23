2020 April 12th

Multithreaded Raytracer

Inspired by
https://www.flipcode.com/archives/Raytracing_Topics_Techniques-Part_1_Introduction.shtml
But mostly made up on the go from snippets around the internet.


2020 April 13th 

First we will implement mathematical 3D-vectors as a class.
Initially some basic arithmetics, more added as needed.

2020 April 14th

Iteratively implementing rays, vectors and primitives.
TO DO:


	1: Overload operators for vectors for dot products and addition etc.

	2: Finish intersect code for Sphere.

	3: Write a scene class

	4: Spawn rays

2020 April 15th

We have tracing! Yay! \o/

![Alt text](test.jpg?raw=true "First trace")

TO DO (Not ordered):


	1: Write proper color handling

	2: Write recursive raytrace

	3: Add lights
	
	4: Add primitive type plane

	5: Add primitive type triangle

	6: Clean up MAKEFILE

	7: Multithreading

	8: Animation

2020 April 17th

Minor detour considering spectral tracing and generating RGB colors from XYZ CIE from wavelengths.
Better documentation started.

![Alt text](docu.png?raw=true "UML")

TO DO (Not ordered):


	1: Write proper color handling

	2: Write recursive raytrace

	3: Add lights
	
	4: Add primitive type plane

	5: Add primitive type triangle

	6: Multithreading

	7: Animation

2020 April 21st

Implemented some functionality for Quats, added planes and did some primitive animation.

![Alt text](ani.gif?raw=true "Primitive animation")

Fixed distance has to be positive. Otherwise collision happens behind viewpoint.
Fixed normalizing Vectors or Quats (when they are 0).

TO DO:

	1: Write proper color handling

	2: Write recursive raytrace

	3: Add point lights

	4: Add rectangle lights

	5: Add shadows

	6: Add primitive type triangle

	7: Multithreading

	8: Animation

2020 April 22nd

Tested recursive (turned into a queue) raytracing. Things are in need of refactoring.
Especially Scene.cpp and everything to do with rays, colors, materials and collisions.
Documentation UML needs to be updated as well and used to plan any further steps.

![Alt text](ani_refl.gif?raw=true "Primitive animation")

TO DO:

	1: Write proper color handling

	2: Add rectangle lights

	3: Add shadows

	4: Add primitive type triangle

	5: Multithreading

2020 April 23rd

Some freestyled color handling was implemented.
Random packets of rays implemented.
Documentation still needs to be updated.
Below is an example (scaled down hi-res render) artifacts are clearly visible.

![Alt text](supersampled.png?raw=true "Primitive animation")

Different amounts of specularity results in different scattering of rays and preservation of color.
This gives objects some kind of "texture" or roughness. We are reaching the limits of ray tracing however.
Pathtracing or cone/beam tracing would be fun to explore.

TO DO:

	1: Add rectangle lights (somewhat initiated with random packets)

	2: Add shadows

	3: Add primitive type triangle

	4: Multithreading

