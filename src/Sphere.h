#ifndef __Sphere_h_
#define __Sphere_h_

#include "Primitive.h"

class Sphere : public Primitive {
public:
	Collision intersect(Ray ray);
private:
	float radius;
};

#endif
