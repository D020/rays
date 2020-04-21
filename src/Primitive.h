#ifndef __Primitive_h_
#define __Primitive_h_

#include "Vec3.h"
#include "Ray.h"

struct Collision {
    Vec3 normal;
    float distance;
};

class Primitive {

friend class Sphere;
friend class Plane;

public:
	Primitive();
	Primitive(Vec3 org);
	virtual void print();
	Vec3 getOrg();

	virtual Collision intersect(Ray ray);

private:
	Vec3 org;
};

#endif
