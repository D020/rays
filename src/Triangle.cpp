#include "Triangle.h"
#include <stdio.h>
#include <limits>
#include <math.h>
#include <algorithm>
Triangle::	Triangle(Vec3 vert1, Vec3 vert2, Vec3 vert3){
	this->vert1 = vert1;
	this->vert2 = vert2;
	this->vert3 = vert3;
	this->norm  = (vert2 - vert1) % (vert3 - vert1);
}

Collision Triangle::intersect(Ray ray){
	//https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

	Collision result;
	Vec3 dir = ray.getDir();
	Vec3 org = ray.getOrg();
	result.normal = -1.0*dir;
		
	Vec3 v0v1 = vert2 - vert1;
	Vec3 v0v2 = vert3 - vert1;
	Vec3 pvec = dir % v0v2; //
	float det = v0v1 * pvec;
	if (fabs(det) < 0.0001){
		result.distance = std::numeric_limits<float>::infinity();
		return result;
	}

	float invDet = 1 / det;
	
	Vec3 tvec = org - vert1;
	float u = (tvec * pvec) * invDet;
	if (u < 0 || u > 1){
		result.distance = std::numeric_limits<float>::infinity();
		return result;
	}

	Vec3 qvec = tvec % v0v1; //
	float v = (dir * qvec) * invDet;
	if (v < 0 || u + v > 1){
		result.distance = std::numeric_limits<float>::infinity();
		return result;
	}
	
	float t = (v0v2 * qvec) * invDet;

	if (t > 0.0001){ // Intersection
		result.distance = (dir*t).length();
		//result.distance = (vert1 + u*edge1 + v*edge2 - ray.getOrg()).length();
			
		float dot = dir*norm;
		float sig = copysign(1.0,dot);
		result.normal = sig*norm;
	}
	else
		result.distance = std::numeric_limits<float>::infinity();

	return result;
}

void Triangle::print(){
	printf("Triangle with V1 V2 V3\n");
	this->vert1.print(false);
	this->vert2.print(false);
	this->vert3.print(false);
}
