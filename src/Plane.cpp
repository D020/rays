#include "Plane.h"
#include <stdio.h>
#include <limits>
#include <math.h>
#include <algorithm>
Plane::Plane(Vec3 org, Vec3 norm){
	this->org    = org;
	this->norm   = norm;
}

Collision Plane::intersect(Ray ray){

	//Wiki Line-Sphere intersection

	Vec3 p0; //Point on the plane
	Vec3 l0; //Point in the line (ray)
	Vec3 l;  //Line direction
	Vec3 n;  //Plane normal

	p0 = this->getOrg();
	n  = -1*this->norm.norm();

	l0 = ray.getOrg();
	l  = ray.getDir().norm();

	Collision result;
	result.normal = -1.0*l;
	result.distance = std::numeric_limits<float>::infinity();

	if(l*n != 0){ //There is an intersection other than parallel contain
		result.distance = ((p0-l0)*n)/(l*n);
		result.normal   = n;
	}

	return result;
}

void Plane::print(){
	printf("Plane located: ");
	this->org.print(false);
	printf("\tNormal:");
	this->norm.print(false);

}
