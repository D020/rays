#ifndef __Plane_h_
#define __Plane_h_

#include "Primitive.h"

class Plane : public Primitive {
public:
	Plane(Vec3 org, Vec3 norm);
	virtual Collision intersect(Ray ray);
	virtual void print();
private:
	Vec3 norm;
};

#endif
