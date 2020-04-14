#ifndef __Ray_h_
#define __Ray_h_

#include "Vec3.h"
class Ray {
public:
	Ray();
	Ray(Vec3 org, Vec3 dir);
	void print();
	Vec3 getOrg();
	Vec3 getDir();
private:
	Vec3 org;
	Vec3 dir;

};

#endif
