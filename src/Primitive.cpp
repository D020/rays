#include "Primitive.h"
#include <stdio.h>

Primitive::Primitive(){
	org = Vec3(0,0,0);
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
