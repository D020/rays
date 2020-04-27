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
int32_t Scene::prng(int32_t seed){
	seed = (seed * 1103515245 + 12345);
	return seed;
}

Scene::Scene(){
	noPrimitives         = 0;
	primitives           = 0;
	horizontalFOVradians = 0;
	  verticalFOVradians = 0;
	rd.state = 0;
	initstate_r(0, state, sizeof(state), &rd);
}

Scene::Scene(int width, int height){
	noPrimitives = 0;
	primitives   = 0;
	rd.state     = 0;
	initstate_r(0, state, sizeof(state), &rd);
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
void Scene::setRays(Vec3 org, Vec3 dir, Plot* plot){
	int height = plot->getHeight();
	int width  = plot->getWidth();

	rays.resize(height*width);

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

		//Set up new reflected rays
		for(int ndx=0; ndx<5; ndx++){

			Vec3 dir;
			do{
			//float theta = 0  + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2*M_PI-0)));
			//float z     = -1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1-(-1))));

			/*
			random_r(&rd, &random);
			float theta = 0  + static_cast <float> (random) /( static_cast <float> (RAND_MAX/(2*M_PI-0)));
			random_r(&rd, &random);
			float z     = -1 + static_cast <float> (random) /( static_cast <float> (RAND_MAX/(1-(-1))));
			*/

			random = prng(random);
			float theta = ((float(random)/float(PRNG_RAND_MAXX))+1)*M_PI;
			random = prng(random);
			float z     = (float(random)/float(PRNG_RAND_MAXX));

			//printf("theta %f z %f\n",theta,z);

			dir = Vec3(sqrt(1-z*z)*cos(theta),sqrt(1-z*z)*sin(theta),z);

			}while(dir*r<0.01 || dir*r < (roughness-0.05));

			dir = (1-roughness)*(dir*0.5) + roughness*(r*0.5);
			dir.norm();

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

			for(int bdx=0; bdx<3; bdx++){
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
