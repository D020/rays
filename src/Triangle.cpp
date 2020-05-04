#include "Triangle.h"
#include <stdio.h>
#include <limits>
#include <math.h>
#include <algorithm>
#include <vector>
using namespace std;
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

Vec3 Triangle::getVert1(){
	return vert1;
}

Vec3 Triangle::getVert2(){
	return vert2;
}

Vec3 Triangle::getVert3(){
	return vert3;
}

bool vertexInBox(Vec3 min, Vec3 max, Vec3 vert){
	if(min.getX()<=vert.getX() && vert.getX()<=max.getX()){
		if(min.getY()<=vert.getY() && vert.getY()<=max.getY()){
			if(min.getZ()<=vert.getZ() && vert.getZ()<=max.getZ())
				return true;
		}
	}
	return false;
}

void Project(vector<Vec3> points, Vec3 axis, float* min, float* max){
    *min = std::numeric_limits<float>::infinity();
    *max = -std::numeric_limits<float>::infinity();

	for(auto& p : points){
		float val = axis * p;
		if (val < *min) *min = val;
		if (*max < val) *max = val;
	}	
}

//https://stackoverflow.com/questions/17458562/efficient-aabb-triangle-intersection-in-c-sharp
bool Triangle::triangleIntersectBox(Vec3 min, Vec3 max){
	float triangleMin, triangleMax;
	float boxMin, boxMax;

	Vec3 boxNormals[3];
	boxNormals[0] = Vec3(1,0,0);
	boxNormals[1] = Vec3(0,1,0);
	boxNormals[2] = Vec3(0,0,1);

	vector<Vec3> triangleVertices;
	triangleVertices.push_back(vert1);
	triangleVertices.push_back(vert2);
	triangleVertices.push_back(vert3);

	float boxStartCoords[3];
	boxStartCoords[0] = min.getX();
	boxStartCoords[1] = min.getY();
	boxStartCoords[2] = min.getZ();

	float boxEndCoords[3];
	boxEndCoords[0] = max.getX();
	boxEndCoords[1] = max.getY();
	boxEndCoords[2] = max.getZ();

	for(int i=0;i<3;i++){
		//Vec3 n = boxNormals[i];
		Project(triangleVertices,boxNormals[i],&triangleMin,&triangleMax);
		if(triangleMax < boxStartCoords[i] || triangleMin > boxEndCoords[i]){
			return false;
		}
	}

	vector<Vec3> triangleEdges;
	triangleEdges.push_back(vert1-vert2);
	triangleEdges.push_back(vert2-vert3);
	triangleEdges.push_back(vert3-vert1);

	Vec3 triangleNormal = (triangleEdges[0] % triangleEdges[1]).norm();

	double triangleOffset = triangleNormal * (vert1);

	vector<Vec3> boxVertices;
	float minX = min.getX();
	float minY = min.getY();
	float minZ = min.getZ();

	float maxX = max.getX();
	float maxY = max.getY();
	float maxZ = max.getZ();
	
	boxVertices.push_back(Vec3(minX,minY,minZ));
	boxVertices.push_back(Vec3(maxX,minY,minZ));
	boxVertices.push_back(Vec3(minX,maxY,minZ));
	boxVertices.push_back(Vec3(maxX,maxY,minZ));
	boxVertices.push_back(Vec3(minX,minY,maxZ));
	boxVertices.push_back(Vec3(maxX,minY,maxZ));
	boxVertices.push_back(Vec3(minX,maxY,maxZ));
	boxVertices.push_back(Vec3(maxX,maxY,maxZ));

	Project(boxVertices, triangleNormal, &boxMin, &boxMax);
    if (boxMax < triangleOffset || boxMin > triangleOffset){
		return false;
	}

	for (int i = 0; i < 3; i++){
    	for (int j = 0; j < 3; j++){
			Vec3 axis = triangleEdges[i] % boxNormals[j];
			Project(boxVertices, axis, &boxMin, &boxMax);
			Project(triangleVertices, axis, &triangleMin, &triangleMax);
			if (boxMax <= triangleMin || boxMin >= triangleMax){
				return false;
			}
		}
	}

	return true;
}

bool Triangle::inBox(Vec3 min, Vec3 max){
	if(vertexInBox(min,max,vert1) || vertexInBox(min,max,vert2) || vertexInBox(min,max,vert3))
		return true;
	return false;
}

void Triangle::print(){
	printf("Triangle with V1 V2 V3\n");
	this->vert1.print(false);
	this->vert2.print(false);
	this->vert3.print(false);
}
