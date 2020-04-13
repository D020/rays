#include "Vec3.h"
#include <math.h>
Vec3::Vec3(){
	x = 0;
	y = 1;
	z = 2;
}

Vec3::Vec3(float x,
		   float y,
		   float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vec3::length(){
	return sqrt(x*x + y*y + z*z);
}

Vec3 Vec3::norm(){
	float len = this->length();
	Vec3 res(x/len,y/len,z/len);
	return res;
}
