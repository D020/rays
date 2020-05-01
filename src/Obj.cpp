#include "Obj.h"
#include <stdio.h>
#include <limits>
#include <math.h>
#include <algorithm>
Obj::Obj(char* path, Vec3 displace, int subd){
	
	char * line = NULL;
	size_t len  = 0;
	ssize_t read;
	FILE* fp = fopen(path,"r");
	float v1,v2,v3;
	int i1,i2,i3;
	vector<Vec3> vecs;

	float min_x = std::numeric_limits<float>::infinity();
	float max_x = -std::numeric_limits<float>::infinity();
	float min_y = std::numeric_limits<float>::infinity();
	float max_y = -std::numeric_limits<float>::infinity();
	float min_z = std::numeric_limits<float>::infinity();
	float max_z = -std::numeric_limits<float>::infinity();

	while(( read = getline(&line, &len, fp)) != -1){
		if(sscanf(line, "v %f %f %f",&v1,&v2,&v3)){
			Vec3 insert = Vec3(v1,v2,v3) + displace;
			vecs.push_back(insert);
	
			
			if(insert.getX()<min_x)
				min_x = insert.getX();
			if(insert.getY()<min_y)
				min_y = insert.getY();
			if(insert.getZ()<min_z)
				min_z = insert.getZ();

			if(max_x<insert.getX())
				max_x = insert.getX();
			if(max_y<insert.getY())
				max_y = insert.getY();
			if(max_z<insert.getZ())
				max_z = insert.getZ();


		}
		if(sscanf(line, "f %i/%*i/%*i %i/%*i/%*i %i/%*i/%*i",&i1,&i2,&i3)){
			Triangle tri(vecs[i1-1],vecs[i2-1],vecs[i3-1]);
			tri.setColor(Vec3(0.476990,0.319510,0.288094));
			tri.setSpecular(0.20);
			tri.setRoughness(0.90);
			tris.push_back(tri);
		}	
	}

	min = Vec3(min_x,min_y,min_z);
	max = Vec3(max_x,max_y,max_z);

	float x_range = max.getX() - min.getX();
	float y_range = max.getY() - min.getY();
	float z_range = max.getZ() - min.getZ();

	printf("miny %f maxy %f",min.getY(),max.getY())

	for(float ysub=0; ysub<subd; ysub++){
		float min_y = (ysub/subd)*y_range + min.getY();
		float max_y = ((ysub+1)/subd)*y_range + min.getY();
		printf("Subbox miny %f, maxy %f\n",min_y,max_y);
	}

}

bool Obj::boxIntersect(Ray ray) { 
//Almost direct copy of:
//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
    float tmin = (min.getX() - ray.getOrg().getX()) / ray.getDir().getX(); 
    float tmax = (max.getX() - ray.getOrg().getX()) / ray.getDir().getX();
 
    if (tmin > tmax) swap(tmin, tmax); 
 
    float tymin = (min.getY() - ray.getOrg().getY()) / ray.getDir().getY(); 
    float tymax = (max.getY() - ray.getOrg().getY()) / ray.getDir().getY(); 
 
    if (tymin > tymax) swap(tymin, tymax); 
 
    if ((tmin > tymax) || (tymin > tmax)) 
        return false; 
 
    if (tymin > tmin) 
        tmin = tymin; 
 
    if (tymax < tmax) 
        tmax = tymax; 
 
    float tzmin = (min.getZ() - ray.getOrg().getZ()) / ray.getDir().getZ(); 
    float tzmax = (max.getZ() - ray.getOrg().getZ()) / ray.getDir().getZ(); 
 
    if (tzmin > tzmax) swap(tzmin, tzmax); 
 
    if ((tmin > tzmax) || (tzmin > tmax)) 
        return false; 
 
    if (tzmin > tmin) 
        tmin = tzmin; 
 
    if (tzmax < tmax) 
        tmax = tzmax; 
 
    return true; 
} 

Collision Obj::intersect(Ray ray){

	Collision colMin;
	colMin.normal   = -1.0*ray.getDir().norm();
	colMin.distance = std::numeric_limits<float>::infinity();

	if(boxIntersect(ray)){
		for(unsigned int pdx=0; pdx<tris.size(); pdx++){
			Collision curCol = tris[pdx].intersect(ray);
			if (curCol.distance < colMin.distance && 0 < curCol.distance){
				colMin.distance = curCol.distance;
				colMin  = curCol;
			}
		}
	}

	return colMin;
}

void Obj::print(){
	printf("Object with displacement:\n");
	this->displace.print(false);
}
