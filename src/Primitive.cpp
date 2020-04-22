#include "Primitive.h"
#include <stdio.h>

Primitive::Primitive(){
	org = Vec3(0,0,0);
	color = Vec3(1.0,1.0,0.0);
	specular = 0.05;
}

Primitive::Primitive(Vec3 org){
	this->org = org;
}

Collision Primitive::intersect(Ray ray){
	Collision result;
	result.normal = Vec3(0,0,0);
	result.distance = 0;
	return result;
}

void Primitive::print(){
	printf("Primitive origin: ");
	org.print(false);
}

Vec3 Primitive::getOrg(){
	return this->org;
}

Vec3 Primitive::getColor(){
	return this->color;
}

float Primitive::getSpecular(){
	return this->specular;
}
