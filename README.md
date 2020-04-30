# Raytracing

Results so far (April 30th):

![Alt text](tris.png?raw=true "Triangles everywhere!")

![Alt text](ani_tris.gif?raw=true "Triangles everywhere!")

Animations further down


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

![Alt text](supersampled.png?raw=true "4k downsized")

![Alt text](ani_super.gif?raw=true "1280 downsized animation")

Different amounts of specularity results in different scattering of rays and preservation of color.
This gives objects some kind of "texture" or roughness. We are reaching the limits of ray tracing however.
Pathtracing or cone/beam tracing would be fun to explore.

TO DO:

	1: Add rectangle lights (somewhat initiated with random packets)

	2: Add shadows

	3: Add primitive type triangle

	4: Multithreading


2020 April 24th

Shadows added. Some kind of attempt at multithreading implemented with disappointing results in terms of peformance gains.

![Alt text](ani_shadows.gif?raw=true "Shadows")

TO DO:

	1: Add rectangle lights (somewhat initiated with random packets)

	2: Add primitive type triangle

	3: Multithreading (Fix)

2020 April 27th

Multithreading fixed. Used oprofile to sample what functions the most time was spent in. Turns out the random number generator (RNG)
from the standard C++ library has locks. Found a simple implementation of LCG RNG. Now proper roughness could be implemented.
The performance gain from multithreading is not noticeable with the poor way that random rays are generated on rough surfaces.
The dot product of a random vector in the unit sphere with the reflected ray is compared to a roughness-factor and if it is not
satisfied, a new random direction is generated. With low roughness surfaces, this can take quite a while.

![Alt text](ani_rough.gif?raw=true "Roughness")

TO DO:

	1: Add rectangle lights (somewhat initiated with random packets)

	2: Add primitive type triangle

	3: Generate random rays in a better way than discarding over and over.

2020 April 30th

Random rays generated properly now. Triangles implemented using: 

https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection?url=3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection

![Alt text](tris.png?raw=true "Triangles")

TO DO:

	1: Add rectangle lights (somewhat initiated with random packets)

