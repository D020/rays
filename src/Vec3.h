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

	friend Vec3  operator + (const Vec3 &a, const Vec3 &b);
	friend Vec3  operator - (const Vec3 &a, const Vec3 &b);
	friend float operator * (const Vec3 &a, const Vec3 &b);
	friend Vec3  operator * (const Vec3 &a, float scalar);
	friend Vec3  operator * (float scalar, const Vec3 &a);

private:
	float x,y,z;
};



#endif
