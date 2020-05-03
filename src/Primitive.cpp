#include "Primitive.h"
#include <stdio.h>

Primitive::Primitive(){
	org = Vec3(0,0,0);
	color = Vec3(1.0,1.0,0.0);
	specular = 0.0;
	roughness = 0.0;
	reflectance = 0.0;
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

void Primitive::setColor(Vec3 color){
	this->color = color;
}

float Primitive::getSpecular(){
	return this->specular;
}

void Primitive::setSpecular(float specular){
	this->specular = specular;
}

float Primitive::getRoughness(){
	return this->roughness;
}

void Primitive::setRoughness(float roughness){
	this->roughness = roughness;
}

float Primitive::getReflectance(){
	return this->reflectance;
}

void Primitive::setReflectance(float reflectance){
	this->reflectance = reflectance;
}
