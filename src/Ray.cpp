#include "Ray.h"
#include <stdio.h>

Ray::Ray(){
	org = Vec3(0,0,0);
	dir = Vec3(0,0,0);
}

Ray::Ray(Vec3 org, Vec3 dir){
	this->org = org;
	this->dir = dir;
}

Vec3 Ray::getOrg(){
	return this->org;
}

Vec3 Ray::getDir(){
	return this->dir;
}

void Ray::print(){
	printf("Origin: ");
	org.print(false);
	printf("Direct: ");
	dir.print(false);
}
