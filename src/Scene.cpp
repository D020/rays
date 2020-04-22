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

			rays[idx] = Ray(org, dir_tmp);
		}
	}

}

SceneCollision Scene::intersect(Ray ray, int ignore){

	Collision colMin;
	colMin.normal   = -1.0*ray.getDir().norm();
	colMin.distance = std::numeric_limits<float>::infinity();

	int primHitIndex = -1;

	for(int pdx=0; pdx<noPrimitives; pdx++){
		if(pdx==ignore)//If we are trying to trace from a surface, we will 
			continue;  //immediatly hit it if its not ignored.
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
	scCol.position = ray.getOrg() + colMin.distance*(ray.getDir().norm());

	return scCol;
}

void Scene::render(const char* path){
	int height = plot.getHeight();
	int width  = plot.getWidth();
	Vec3 light(-4,-4,4);
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){

			unsigned int idx = i*width+j;
			Ray ray = rays[idx];

			int bounces = 2;

			float intensity[10];
			float specular [10];
			Vec3  color    [10];
			float rs[10];
			float gs[10];
			float bs[10];
			int ignoreIndex = -1;

			for(int rdx=0; rdx<bounces; rdx++){
				SceneCollision col = this->intersect(ray, ignoreIndex);

				//https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector

				Vec3 n = col.normal.norm();
				Vec3 d = ray.getDir().norm();
				Vec3 r = d - 2*(d*n)*n;

				Vec3 toLight = light - col.position;

				intensity[rdx] = toLight.norm()*r;

				Vec3 color;
				color = primitives[col.hitIndex]->getColor();
		
				rs[rdx]       = color.getX();
				gs[rdx]       = color.getY();
				bs[rdx]       = color.getZ();
				specular[rdx] = primitives[col.hitIndex]->getSpecular();

				//Set up new reflected ray
				ray = Ray(col.position,r);
				ignoreIndex = col.hitIndex;
			}

			float final_r = 0;
			float final_g = 0;
			float final_b = 0;

			final_r += rs[0]*intensity[0];
			final_g += gs[0]*intensity[0];
			final_b += bs[0]*intensity[0];			

			for(int rdx=1; rdx<bounces; rdx++){
				final_r += rs[rdx]*intensity[rdx]*specular[rdx-1];
				final_g += gs[rdx]*intensity[rdx]*specular[rdx-1];
				final_b += bs[rdx]*intensity[rdx]*specular[rdx-1];
			}
			
			//final_r/=bounces;
			//final_g/=bounces;
			//final_b/=bounces;

			plot.plot(j,i,final_r,final_g,final_b);
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
