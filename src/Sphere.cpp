#include "Sphere.h"

Collision Sphere::intersect(Ray ray){

	//Wiki Line-Sphere intersection

	float d; 	//Distance
	Vec3 l; 	//Direction of line (unit vector)
	Vec3 o; 	//Origin of line;
	Vec3 c; 	//Origin of sphere;

	l = ray.getDir();
	o = ray.getOrg();
	c = this->getOrg();
	
	//under_sqrt = (l*(o-c))**2 - 

	Collision result;

	return result;
}
