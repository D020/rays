#ifndef __Sphere_h_
#define __Sphere_h_

#include "Primitive.h"

class Sphere : public Primitive {
public:
	Sphere(Vec3 org, float radius);
	Collision intersect(Ray ray);
	void print();
private:
	float radius;
};

#endif
