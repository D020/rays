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
		float intensityDiff;
		float intensitySpec;

		//Reflected color based on specularity that controls
		//how fussy or sharp the cone of reflected rays are
		float specular  = primitives[col.hitIndex]->getSpecular();
		float roughness = primitives[col.hitIndex]->getRoughness();

		if(shadeCol.distance<toLight.length()){
			intensityDiff = 0;
			intensitySpec = 0;
		}
		else{
			float dot = (-1*toLight.norm()*n) > 0 ? (-1*toLight.norm()*n) : 0;

			intensityDiff = lightPower * dot/(toLight.length()*toLight.length());
			intensitySpec = lightPower * pow(dot,specular*8)/(toLight.length()*toLight.length());
		}
		//Otherwise it's given a color with intensity according to the dot product scaled by distance to the light.
		Vec3 color;
		color = intensityDiff * primitives[col.hitIndex]->getColor(); //+ intensitySpec * primitives[col.hitIndex]->getColor();


		Quat R = rotateFromTo(Vec3(0,0,1),r);

		float fallOff = (col.distance+1)*(col.distance+1);

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


void Scene::render(int cores,int sz, Plot* plot){
	float height = plot->getHeight();
	float width  = plot->getWidth();

	int tiles_height = ceil(height/float(sz));
	int tiles_width = ceil(width/float(sz));

	printf("%i %i\n",tiles_height,tiles_width);

	vector<int> xas;
	vector<int> xbs;
	vector<int> yas;
	vector<int> ybs;

	//First we set up all the tiles from-to x and y values	
	
	for(int ty=0; ty<tiles_height; ty++){
		int ya = ty*sz;
		int yb = (ty+1)*sz;
		if(height<yb)
			yb = height;
		for(int tx=0; tx<tiles_width; tx++){
			int xa = tx*sz;
			int xb = (tx+1)*sz;
			if(width<xb)
				xb = width;
			xas.push_back(xa);
			xbs.push_back(xb);
			yas.push_back(ya);
			ybs.push_back(yb);
		}
	}
	
	//Then we set up a bool-array that can tell us whether
	//a thread is done with its 32x32 tiles or not such
	//that we may spawn a new one.

	bool* done = (bool*) calloc(cores,sizeof(bool));
	memset(done,true,cores);
	bool* tile_done = (bool*) calloc(xas.size(),sizeof(bool));

	unsigned int tiles_dispatched = 0;
	printf("\e[1;1H\e[2J");
	while(tiles_dispatched<xas.size()){
		for(int cdx=0; cdx<cores; cdx++){
			if(tiles_dispatched==xas.size())
				break;
			if(done[cdx]==true){
				done[cdx] = false;
				int xa = xas[tiles_dispatched];
				int xb = xbs[tiles_dispatched];
				int ya = yas[tiles_dispatched];
				int yb = ybs[tiles_dispatched];		
				thread ps(&Scene::renderPart, this,ya,yb,xa,xb,plot,&done,cdx, &tile_done, tiles_dispatched);
				ps.detach();
				tiles_dispatched++;
			}
		}		
		printf("\033[%d;%dH", 0, 0);
		printf("Tiles dispatched %u / %lu\n",tiles_dispatched,xas.size());
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	bool total_done;

	do{
		total_done = true;
		for(unsigned int tdx=0;tdx<xas.size();tdx++)
			total_done = total_done && tile_done[tdx];
	}while(!total_done);
	

}

void Scene::renderPart(int ya, int yb, int xa, int xb, Plot* plot, bool** done, int tdx, bool** tile_done, int tile){

	int width  = plot->getWidth();
	for(int i=ya; i<yb; i++){
		for(int j=xa; j<xb; j++){
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
	}

	(*done)[tdx] = true;
	(*tile_done)[tile] = true;

}

void Scene::print(){
	printf("Scene contains:\n");
	for(int i=0; i<noPrimitives; i++){
		primitives[i]->print();
	}
}
