#include "Scene.h"
#include "Quat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <limits>
#include <thread>

#define PRNG_RAND_MAX  0xFFFFFFFF
#define PRNG_RAND_MAXX 0x7FFFFFFF
int32_t Scene::prng(){
	random = (random * 1103515245 + 12345);
	return random;
}

float Scene::prng_range(float a, float b){
	int32_t random = prng();
	float res = (float(random)/float(PRNG_RAND_MAXX)); //Between -1 and +1
	      res = (res + 1)/2; //Between 0 and 1
		  res = res*(b-a);   //Between 0 and (the distance between a and b)
		  res = res + a;     //Between a and b
	return res;
}

Scene::Scene(){
	noPrimitives         = 0;
	primitives           = 0;
	horizontalFOVradians = 0;
	  verticalFOVradians = 0;
	random               = 0;
}

Scene::Scene(int width, int height){
	noPrimitives = 0;
	primitives   = 0;
	random       = 0;
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
//https://math.stackexchange.com/questions/40164/how-do-you-rotate-a-vector-by-a-unit-quaternion

Quat rotateFromTo(Vec3 v1, Vec3 v2){
	v1 = v1.norm();
	v2 = v2.norm();

	Vec3 a;
	float w;
	if(0.999<v1*v2 || v1*v2<-0.999){
		a = Vec3(0,0,0);}
	else{
		a = v1 % v2;}
	
	w = sqrt((v1*v1)*(v2*v2)) + v1*v2;	

	Quat R(w,a.getX(),a.getY(),a.getZ());

	return R.norm();

}

Vec3 applyQuatRot(Quat R, Vec3 a){
	Quat Rc = R.conj();

	Quat P;
	P = Quat(0,a.getX(),a.getY(),a.getZ());
	P = (R*P)*Rc;
			
	return P.axis();
}

void Scene::setRays(Vec3 org, Vec3 dir, Plot* plot){
	int height = plot->getHeight();
	int width  = plot->getWidth();

	rays.resize(height*width);

	Vec3 centerRay(0,0,org.getZ()+8);

	Quat R = rotateFromTo(centerRay,dir);

	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){

			float x = -9.0 + (16.0/float(width ))*j;
			float y =  4.5 - (9.0 /float(height))*i;

			unsigned int idx = i*width+j;

			Vec3 P;
			P = Vec3(x,y,org.getZ()+8) - org;
			Vec3 dir_tmp = applyQuatRot(R,P);

			rays[idx] = Ray(org, dir_tmp);
		}
	}

}

void Scene::setLight(Vec3 light){
	this->light = light;
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

vector<Ray> Scene::SceneTraceBundle(vector<Ray> rays){
	vector<SceneCollision> cols;
	vector<Ray> newRays;
	
	float lightPower = 200;

	for(unsigned int rdx=0; rdx<rays.size(); rdx++){
		Ray ray_tmp = rays[rdx];

		//Dirty self-collide hack
		Ray ray(ray_tmp.getOrg()+ray_tmp.getDir()*0.001,ray_tmp.getDir(),ray_tmp.getColor(),ray_tmp.getPropColor());

		SceneCollision col = this->intersect(ray, -1);
		
		if(col.hitIndex<0)
			return newRays;

		//Calculate the reflected vector r based on normal of the surface n and the incoming vector d.
		//https://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
		Vec3 n = col.normal.norm();
		Vec3 d = ray.getDir().norm();
		Vec3 r = d - 2*(d*n)*n;


		//Is this point in the shadow of another object?
		Vec3 toLight = light - col.position;
		Ray shadeRay(col.position + toLight.norm()*0.001,toLight.norm());
		SceneCollision shadeCol = this->intersect(shadeRay,-1);
		float intensity;
		if(shadeCol.distance<toLight.length())
			intensity = 0;
		else
			intensity = lightPower * (-1*toLight.norm()*n)/(toLight.length()*toLight.length());
		//Otherwise it's given a color with intensity according to the dot product scaled by distance to the light.
		Vec3 color;
		color = intensity * primitives[col.hitIndex]->getColor();


		//Reflected color based on specularity that controls
		//how fussy or sharp the cone of reflected rays are
		float specular  = primitives[col.hitIndex]->getSpecular();
		float roughness = primitives[col.hitIndex]->getRoughness();

		Quat R = rotateFromTo(Vec3(0,0,1),r);

		//Set up new reflected rays
		for(int ndx=0; ndx<5; ndx++){

			Vec3 dir;

			//https://math.stackexchange.com/questions/56784/generate-a-random-direction-within-a-cone

			
			float angle = (1-roughness) * M_PI/2;
			float z     = prng_range(cos(angle),1);
			float theta = prng_range(0,2*M_PI);

			dir = Vec3(sqrt(1-z*z)*cos(theta),sqrt(1-z*z)*sin(theta),z);
			dir = applyQuatRot(R,dir).norm();

			ray = Ray(col.position,dir,
					  color*ray.getPropColor() + ray.getColor()*(1-ray.getPropColor()),
                      specular);
		
			//ignoreIndex = col.hitIndex;

			newRays.push_back(ray);
		}
	}
	

	return newRays;	
}

/*
void Scene::render(int cores, Plot* plot){
	
	int height = plot->getHeight();

    std::thread t1(&Scene::renderPart, this,  0,       height/2, plot);
    std::thread t2(&Scene::renderPart, this, height/2, height,   plot);
    t1.join();
    t2.join();
}
*/


void Scene::render(int cores, Plot* plot){
	int height = plot->getHeight();

	vector<thread> pss;

	int interval = height/cores;

	for(int cdx=0; cdx<cores;cdx++){
		thread ps(&Scene::renderPart, this, floor(interval*cdx), floor(interval*(cdx+1)), plot);
		int from = interval*cdx;
		int to   = interval*(cdx+1);
		
		//pss.emplace_back([&]{renderPart(from,to);});
		pss.push_back(move(ps));
		printf("From %i, To %i\n",from,to);
	}
	for(int cdx=0; cdx<cores;cdx++){
		pss[cdx].join();
	}

}

void Scene::renderPart(int ya, int yb, Plot* plot){
	int width  = plot->getWidth();
	for(int i=ya; i<yb; i++){
		for(int j=0; j<width; j++){

			unsigned int idx = i*width+j;
			Ray rayIn = rays[idx];

			vector<Ray> raysIn;
			raysIn.push_back(rayIn);

			for(int bdx=0; bdx<2; bdx++){
				raysIn = SceneTraceBundle(raysIn);
			}

			Vec3 finalRayColor(0,0,0);
			int noRays = raysIn.size();
			for(int ndx=0;ndx<noRays;ndx++)
				finalRayColor = finalRayColor + raysIn[ndx].getColor();
			
			finalRayColor = finalRayColor * (1/float(noRays));

			plot->plot(j,i,finalRayColor.getX(),finalRayColor.getY(),finalRayColor.getZ());
		}
		if(ya==0)
			printf("%i/%i\n",i,yb);
	}
}

void Scene::print(){
	printf("Scene contains:\n");
	for(int i=0; i<noPrimitives; i++){
		primitives[i]->print();
	}
}
