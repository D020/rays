#include "Sphere.h"
#include <stdio.h>
#include <limits>
#include <math.h>
#include <algorithm>
Sphere::Sphere(Vec3 org, float radius){
	this->org    = org;
	this->radius = radius;
}

Collision Sphere::intersect(Ray ray){

	//Wiki Line-Sphere intersection

	float d; 	//Distance
	Vec3  l; 	//Direction of line (unit vector)
	Vec3  o; 	//Origin of line;
	Vec3  c; 	//Origin of sphere;
	float r;	//Radius of sphere;

	l = ray.getDir().norm();
	o = ray.getOrg();
	c = this->getOrg();
	r = this->radius;

	float under_sqrt = ( (l*(o-c)) * (l*(o-c)) ) - ((o-c)*(o-c) - r*r);

	Collision result;
	result.normal = -1.0*l;

	if (under_sqrt < 0){ //Then there is no intersection
		result.distance = std::numeric_limits<float>::infinity();
	}
	else{				 //There is intersection or tangent.
		float tmp 			= -1*(l*(o-c));
		float intersection1 = tmp + sqrt(under_sqrt);
		float intersection2 = tmp - sqrt(under_sqrt);
		
		result.distance     = fmin(intersection1,intersection2);
		Vec3 intersectPoint = (o+(l*result.distance));
		result.normal       = (c-intersectPoint).norm();
	}

	return result;
}

void Sphere::print(){
	printf("Sphere with r=%f located at: ",this->radius);
	this->org.print(false);
}
