#include "Scene.h"
#include "Quat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <limits>
#include <thread>
#include <atomic>

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
	if(0.999999<v1*v2 || v1*v2<-0.999999){
        return Quat(1,0,0,0);
    }
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

void Scene::moveLight(int ldx, Vec3 org){
	point_lights[ldx] = org;
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

bool Scene::shadeTrace(Vec3 toLight, Vec3 position){
	Ray shadeRay(position + toLight.norm()*0.001,toLight.norm());
	SceneCollision shadeCol = this->intersect(shadeRay,-1);

	if(shadeCol.distance<toLight.length()){
		return true;
	}
	else{
		return false;
	}
}

vector<Ray> Scene::SceneTraceBundle(vector<Ray> rays, int bounce){
	vector<SceneCollision> cols;
	vector<Ray> newRays;

	for(auto& ray_tmp : rays){

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

		//########## LIGHT #############
		Vec3 color;
		for(unsigned int ldx=0; ldx<point_lights.size();ldx++){
			float lightPower = point_lightspower[ldx];

			//Is this point in the shadow of another object?
			Vec3 toLight = point_lights[ldx] - col.position;
			float intensityDiff;
			//float intensitySpec;
			if(shadeTrace(toLight, col.position)){;
				intensityDiff = 0;
				//intensitySpec = 0;
			}
			else{//Otherwise it's given a color with intensity according to the dot product scaled by distance to the light.
				float fallOff = toLight.length()*toLight.length();
				float dot = (-1*toLight.norm()*n) > 0 ? (-1*toLight.norm()*n) : 0;
				intensityDiff = lightPower * dot/fallOff;
				//intensitySpec = lightPower * pow(dot,8)/fallOff;
			}

			color = color + intensityDiff * primitives[col.hitIndex]->getColor();
		}
		
        for(unsigned int ldx=0; ldx<surf_lights_center.size();ldx++){
			float lightPower = surf_lightspower[ldx];

            float intensityDiff = 0;
            for(int pdx=0; pdx<10; pdx++){
                //Is this point in the shadow of another object?
                float u = prng_range(-1,1);
                float v = prng_range(-1,1);
                Vec3 toLight = (surf_lights_center[ldx] + u*surf_lights_edge1[ldx] + v*surf_lights_edge2[ldx]) - col.position;
                //float intensitySpec;
                if(shadeTrace(toLight, col.position)){;
                    intensityDiff += 0;
                    //intensitySpec = 0;
                }
                else{//Otherwise it's given a color with intensity according to the dot product scaled by distance to the light.
                    float fallOff = toLight.length()*toLight.length();
                    float dot = (-1*toLight.norm()*n) > 0 ? (-1*toLight.norm()*n) : 0;
                    intensityDiff += lightPower * dot/fallOff;
                    //intensitySpec = lightPower * pow(dot,8)/fallOff;
                }
            }
            
            intensityDiff = intensityDiff/10;

			color = color + intensityDiff * primitives[col.hitIndex]->getColor();
		}
		
		
		//######### END OF LIGHT #############

		//float specular  = primitives[col.hitIndex]->getSpecular();
		float roughness = primitives[col.hitIndex]->getRoughness();
		float reflectance = primitives[col.hitIndex]->getReflectance();
		Quat R = rotateFromTo(Vec3(0,0,1),r);

		//https://math.stackexchange.com/questions/56784/generate-a-random-direction-within-a-cone
		//Set up new reflected rays
		//Reflected color based on reflectance and roughness that controls
		//how fussy or sharp the cone of reflected rays are
		for(int ndx=0; ndx<5-bounce*2; ndx++){

			Vec3 dir;
			
			float angle = roughness * M_PI/2;
			float z     = prng_range(cos(angle),1);
			float theta = prng_range(0,2*M_PI);

			dir = Vec3(sqrt(1-z*z)*cos(theta),sqrt(1-z*z)*sin(theta),z);
			dir = applyQuatRot(R,dir).norm();

			ray = Ray(col.position,dir,
					  color*ray.getPropColor() + ray.getColor()*(1-ray.getPropColor()), //Vec3 color
                      reflectance*ray.getPropColor());									//float propColor

			newRays.push_back(ray);
		}
	}
	

	return newRays;	
}


void Scene::render(int cores,int sz, Plot* plot){
	float height = plot->getHeight();
	float width  = plot->getWidth();

	int tiles_height = ceil(height/float(sz));
	int tiles_width = ceil(width/float(sz));

	printf("%i %i\n",tiles_height,tiles_width);

	vector<int> xas;
	vector<int> yas;

	//First we set up all the tiles from-to x and y values	
	
	for(int ty=0; ty<tiles_height; ty++){
		int ya = ty*sz;
		for(int tx=0; tx<tiles_width; tx++){
			int xa = tx*sz;
			xas.push_back(xa);
			yas.push_back(ya);
		}
	}
	

	std::vector<std::thread> threads(cores);
	std::atomic<size_t> idxNextTile { 0 };
	const auto nTiles = xas.size();
	int nThread = 0;
	for (auto& thread : threads)
	{
		auto myCdx = nThread++;
		thread = std::thread([&, myCdx]()
		{
			for (size_t idxTile = idxNextTile++; idxTile < nTiles; idxTile = idxNextTile++)
			{
				int xa = xas[idxTile];
				int xb = xa + sz;
				if(width<xb)
					xb = width;
				int ya = yas[idxTile];
				int yb = ya + sz;
				if(height<yb)
					yb = height;
				renderPart(ya,yb,xa,xb,plot);
			}
		});
	}

	printf("\e[1;1H\e[2J");
	for (size_t idxTile = idxNextTile; idxTile < nTiles; idxTile = idxNextTile)
	{
		printf("\033[%d;%dH", 0, 0);
	 	printf("Tiles dispatched %lu / %lu\n",idxTile,nTiles);
	 	std::this_thread::sleep_for(std::chrono::milliseconds(5));
		
	}

	for (auto& thread : threads)
		thread.join();
	
}

void Scene::renderPart(int ya, int yb, int xa, int xb, Plot* plot){

	int width  = plot->getWidth();
	for(int i=ya; i<yb; i++){
		for(int j=xa; j<xb; j++){
			unsigned int idx = i*width+j;
			Ray rayIn = rays[idx];

			vector<Ray> raysIn;
			raysIn.push_back(rayIn);

			for(int bdx=0; bdx<3; bdx++){
				raysIn = SceneTraceBundle(raysIn,bdx);
			}

			Vec3 finalRayColor(0,0,0);
			int noRays = raysIn.size();
			for(int ndx=0;ndx<noRays;ndx++)
				finalRayColor = finalRayColor + raysIn[ndx].getColor();
			
			finalRayColor = finalRayColor * (1/float(noRays));

			plot->plot(j,i,finalRayColor.getX(),finalRayColor.getY(),finalRayColor.getZ());
		}
	}
}

void Scene::addPointLight(Vec3 pos, float power){
	point_lights.push_back(pos);
	point_lightspower.push_back(power);
}

void Scene::addSurfLight(Vec3 center, Vec3 edge1, Vec3 edge2, float power){
    surf_lights_center.push_back(center);
    surf_lights_edge1.push_back(edge1);
    surf_lights_edge2.push_back(edge2);
    surf_lightspower.push_back(power);
}

void Scene::print(){
	printf("Scene contains:\n");
	for(int i=0; i<noPrimitives; i++){
		primitives[i]->print();
	}
}
