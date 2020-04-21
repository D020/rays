#include "Quat.h"
#include <math.h>
#include <stdio.h>
Quat::Quat(){
	w = 0;
	x = 0;
	y = 0;
	z = 0;
}

Quat::Quat(float w,
		   float x,
		   float y,
		   float z){
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

Quat& Quat::operator = (const Quat &other){
	this->w = other.w;
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

void Quat::print(bool verbose){
	printf("w, x, y, z = %f, %f, %f, %f\n", w, x, y, z);
	if(verbose){
		printf("len = %f\n\n",this->length());
	}
}

float Quat::length(){
	return sqrt(w*w + x*x + y*y + z*z);
}

Quat Quat::norm(){
	float len = this->length();
	Quat res;
	if(len!=0)
		res = Quat(w/len, x/len, y/len, z/len);
	else
		res = Quat(0,0,0,0);
	return res;
}

Quat Quat::conj(){
	Quat res(w, -x, -y, -z);
	return res;
}

Vec3 Quat::axis(){
	Vec3 res(x, y, z);
	return res;
}

Quat operator + (const Quat &a, const Quat &b) { 
	Quat res;
	res.w = a.w + b.w;
	res.x = a.x + b.x;
	res.y = a.y + b.y;
	res.z = a.z + b.z;
	return res; 
} 

Quat operator - (const Quat &a, const Quat &b) { 
	Quat res;
	res.w = a.w - b.w;
	res.x = a.x - b.x;
	res.y = a.y - b.y;
	res.z = a.z - b.z;
	return res; 
} 


//Hamilton Product on Wiki
Quat operator * (const Quat &a, const Quat &b) { 
	Quat res;
	res.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
	res.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;	
	res.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
	res.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
	return res; 
} 

Quat operator * (const Quat &a, float scalar) { 
	Quat res;
	res.w = a.w * scalar;
	res.x = a.x * scalar;
	res.y = a.y * scalar;
	res.z = a.z * scalar;
	return res;
} 

Quat operator * (float scalar, const Quat &a) { 
	Quat res;
	res.w = a.w * scalar;
	res.x = a.x * scalar;
	res.y = a.y * scalar;
	res.z = a.z * scalar;
	return res;
} 
