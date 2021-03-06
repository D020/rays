#ifndef __Ray_h_
#define __Ray_h_

#include "Vec3.h"
class Ray {
public:
	Ray();
	Ray(Vec3 org, Vec3 dir);
	Ray(Vec3 org, Vec3 dir, Vec3 color, float propColor);
	void print();
	Vec3 getOrg();
	Vec3 getDir();
	Vec3 getColor();
	float getPropColor();
private:
	Vec3 org;
	Vec3 dir;
	Vec3 color;
	float propColor;

};

#endif
