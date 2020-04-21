#include "Scene.h"
#include "Quat.h"
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


//Rays are spawned according to the following procedure:
// 1. Pick a single spawn point
// 2. Pick a destination point based on a 16:9 aspect ratio grid
// 3. Generate a quaternion that encodes a rotation that aligns it with dir
// 4. Convert quaternion to matrixmultiplication
// 5. Apply to all rays

//https://stackoverflow.com/questions/1171849/finding-quaternion-representing-the-rotation-from-one-vector-to-another
void Scene::setRays(Vec3 org, Vec3 dir){
	int height = plot.getHeight();
	int width  = plot.getWidth();

	Vec3 centerRay(0,0,org.getZ()+8);

	centerRay = centerRay.norm();
	dir       = dir.norm();

	//Rotation from centerRay to dir
	//Crossproduct is not "defined" when dot<-0.999 or 0.999<dot
	Vec3 a;
	float w;
	if(0.999<centerRay*dir || centerRay*dir<-0.999){
		a = Vec3(0,0,0);}
	else{
		a = centerRay % dir;}
	
	w = sqrt((centerRay*centerRay)*(dir*dir)) + centerRay*dir;

	//https://math.stackexchange.com/questions/40164/how-do-you-rotate-a-vector-by-a-unit-quaternion

	Quat R(w,a.getX(),a.getY(),a.getZ());
	printf("W %f\n",centerRay*dir);
	R.print(false);


	R = R.norm();
	Quat Rc = R.conj();

	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){

			float x = -9.0 + (16.0/float(width ))*j;
			float y =  4.5 - (9.0 /float(height))*i;

			unsigned int idx = i*width+j;
			Quat P;
			P = Quat(0,x,y,org.getZ()+8) - Quat(0,org.getX(),org.getY(),org.getZ());
			P = (R*P)*Rc;
			
			Vec3 dir_tmp = P.axis();


			//TO DO SPAWN RAYS
			rays[idx] = Ray(org, dir_tmp);
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
		if (curCol.distance < colMin.distance && 0 < curCol.distance){
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

			float intensity = col.normal * Vec3(0,1,1).norm();//ray.getDir().norm();
			plot.plot(j,i,intensity,0,0);
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
