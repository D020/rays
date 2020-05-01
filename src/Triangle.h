#ifndef __Triangle_h_
#define __Triangle_h_

#include "Primitive.h"

class Triangle : public Primitive {
public:
	Triangle(Vec3 vert1, Vec3 vert2, Vec3 vert3);
	virtual Collision intersect(Ray ray);
	virtual void print();
	Vec3 getVert1();
	Vec3 getVert2();
	Vec3 getVert3();
	bool inBox(Vec3 min, Vec3 max);
private:
	Vec3 vert1;
	Vec3 vert2;
	Vec3 vert3;
	Vec3 norm;
};

#endif
