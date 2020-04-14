#ifndef __Vec3_h_
#define __Vec3_h_

class Vec3 {
public:
	
	//Constructors + debug
	Vec3();
	Vec3(float x,
		 float y,
		 float z);
	void print(bool verbose);

	//Arithmetics
	float length();
	Vec3 norm();

private:
	float x,y,z;
};

#endif
