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
friend class Triangle;

public:
	Primitive();
	Primitive(Vec3 org);
	virtual void print();
	Vec3 getOrg();
	Vec3 getColor();
	void setColor(Vec3 color);
	float getSpecular();
	void setSpecular(float specular);
	float getRoughness();
	void setRoughness(float roughness);

	virtual Collision intersect(Ray ray);

private:
	Vec3 org;
	Vec3 color;
	float specular;
	float roughness;
};

#endif
