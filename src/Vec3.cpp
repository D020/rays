#include "Vec3.h"
#include <math.h>
#include <stdio.h>
Vec3::Vec3(){
	x = 0;
	y = 0;
	z = 0;
}

Vec3::Vec3(float x,
		   float y,
		   float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

Vec3& Vec3::operator = (const Vec3 &other){
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

void Vec3::print(bool verbose){
	printf("x, y, z = %f, %f, %f\n", x, y, z);
	if(verbose){
		printf("len = %f\n\n",this->length());
	}
}

float Vec3::length(){
	return sqrt(x*x + y*y + z*z);
}

Vec3 Vec3::norm(){
	float len = this->length();
	Vec3 res(x/len,y/len,z/len);
	return res;
}

Vec3 operator + (const Vec3 &a, const Vec3 &b) { 
	Vec3 res;
	res.x = a.x + b.x;
	res.y = a.y + b.y;
	res.z = a.z + b.z;
	return res; 
} 

Vec3 operator - (const Vec3 &a, const Vec3 &b) { 
	Vec3 res;
	res.x = a.x - b.x;
	res.y = a.y - b.y;
	res.z = a.z - b.z;
	return res; 
} 

float operator * (const Vec3 &a, const Vec3 &b) { 
	float res = 0;
	res += a.x * b.x;
	res += a.y * b.y;
	res += a.z * b.z;
	return res; 
} 

Vec3 operator * (const Vec3 &a, float scalar) { 
	Vec3 res;
	res.x = a.x * scalar;
	res.y = a.y * scalar;
	res.z = a.z * scalar;
	return res;
} 

Vec3 operator * (float scalar, const Vec3 &a) { 
	Vec3 res;
	res.x = a.x * scalar;
	res.y = a.y * scalar;
	res.z = a.z * scalar;
	return res;
} 
