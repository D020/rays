#ifndef __Quat_h_
#define __Quat_h_
#include "Vec3.h"
class Quat {
public:
	
	//Constructors + debug
	Quat();
	Quat(float w,
		 float x,
		 float y,
		 float z);
	Quat& operator = (const Quat &other);
	void print(bool verbose);

	//Arithmetics
	float length();
	Quat norm();
	Quat conj();
	Vec3 axis();

	friend Quat  operator + (const Quat &a, const Quat &b);
	friend Quat  operator - (const Quat &a, const Quat &b);
	friend Quat  operator * (const Quat &a, const Quat &b);
	friend Quat  operator * (const Quat &a, float scalar);
	friend Quat  operator * (float scalar, const Quat &a);

private:
	float w,x,y,z;
};



#endif
