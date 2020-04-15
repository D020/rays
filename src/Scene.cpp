#include "Scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <limits>
Scene::Scene(){
	noPrimitives         = 0;
	primitives           = 0;
	rays		         = 0;
	plot 		         = Plot();
	horizontalFOVradians = 0;
	  verticalFOVradians = 0;
}

Scene::Scene(int width, int height){
	noPrimitives = 0;
	primitives   = 0;
	plot 		 = Plot(width, height);
	rays 		 = (Ray*) calloc(width*height,sizeof(Ray));
}

void Scene::addPrimitive(Primitive* prim){
	noPrimitives ++;
	
	Primitive** old_ptr = primitives;
	Primitive** new_ptr = (Primitive**) calloc(noPrimitives,sizeof(Primitive*));

	if(old_ptr == 0){
		old_ptr = new_ptr;
	}
	else{
		memcpy(new_ptr,old_ptr,(noPrimitives-1)*sizeof(Primitive*));
		free(old_ptr);
	}

	new_ptr[noPrimitives-1] = prim;
	primitives 			    = new_ptr;

}

void Scene::setRays(Vec3 org, Vec3 dir){
	int height = plot.getHeight();
	int width  = plot.getWidth();
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){

			float x = -5.0 + (10.0/float(width ))*j;
			float y = -2.5 + (5.0 /float(height))*i;

			unsigned int idx = i*width+j;
			Vec3 dir2;
			dir2 = Vec3(x,y,-2) - org;
			//TO DO SPAWN RAYS
			rays[idx] = Ray(org, dir2);
		}
	}

}

SceneCollision Scene::intersect(Ray ray){

	Collision colMin;
	colMin.normal   = -1.0*ray.getDir().norm();
	colMin.distance = std::numeric_limits<float>::infinity();

	int primHitIndex = -1;

	for(int pdx=0; pdx<noPrimitives; pdx++){
		Collision curCol = primitives[pdx]->intersect(ray);
		if (curCol.distance < colMin.distance){
			colMin.distance = curCol.distance;
			colMin  = curCol;

			primHitIndex = pdx;
		}
	}

	SceneCollision scCol;
	scCol.distance = colMin.distance;
	scCol.normal   = colMin.normal;
	scCol.hitIndex = primHitIndex;

	return scCol;
}

void Scene::render(const char* path){
	int height = plot.getHeight();
	int width  = plot.getWidth();
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){

			unsigned int idx = i*width+j;
			Ray ray = rays[idx];

			SceneCollision col = this->intersect(ray);

			float intensity = col.normal * ray.getDir().norm();
			plot.plot(j,i,intensity*255,0,0);
		}
	}
	plot.save(path);
}

void Scene::print(){
	printf("Scene contains:\n");
	for(int i=0; i<noPrimitives; i++){
		primitives[i]->print();
	}
}
