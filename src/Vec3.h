#ifndef __Vec3_h_
#define __Vec3_h_

class Vec3 {
public:
	Vec3();
	Vec3(float x,
		 float y,
		 float z);

	float length();
	Vec3 norm();
	void print(bool verbose);
private:
	float x,y,z;
};

#endif
