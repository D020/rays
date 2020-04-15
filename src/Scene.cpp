#include "Scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			idx = i*width+j;
			
			//TO DO SPAWN RAYS
			rays[idx] = Ray(org, 
		}
	}

}

void Scene::print(){
	printf("Scene contains:\n");
	for(int i=0; i<noPrimitives; i++){
		primitives[i]->print();
	}
}
