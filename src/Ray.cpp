#include "Ray.h"
#include <stdio.h>

Ray::Ray(){
	org = Vec3(0,0,0);
	dir = Vec3(0,0,0);
	color = Vec3(1,1,1);
	propColor = 1.0;
}

Ray::Ray(Vec3 org, Vec3 dir){
	this->org = org;
	this->dir = dir;
	this->color = Vec3(1,1,1);
	this->propColor = 1.0;
}

Ray::Ray(Vec3 org, Vec3 dir, Vec3 color, float propColor){
	this->org = org;
	this->dir = dir;
	this->color = color;
	this->propColor = propColor;
}

Vec3 Ray::getOrg(){
	return this->org;
}

Vec3 Ray::getDir(){
	return this->dir;
}

float Ray::getPropColor(){
	return propColor;
}

Vec3 Ray::getColor(){
	return color;
}

void Ray::print(){
	printf("Origin: ");
	org.print(false);
	printf("Direct: ");
	dir.print(false);
}
