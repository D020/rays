#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "src/Vec3.h"
#include "src/Quat.h"
#include "src/Plot.h"
#include "src/Ray.h"
#include "src/Primitive.h"
#include "src/Sphere.h"
#include "src/Plane.h"
#include "src/Triangle.h"
#include "src/Scene.h"
#include <math.h>
#include <algorithm>
#include <thread>
using namespace std;


int main() {

	Plot plot(1920,1080);
	Scene testscene(1280,720);

	printf("\n\n\n\n\n");

	Sphere ball1(Vec3(0 ,0,0),2);	
	ball1.setColor(Vec3(0,0,1.0));
	ball1.setSpecular(0.0);	

	Sphere ball2(Vec3(3 ,1,0),2.5);
	ball2.setColor(Vec3(1.0,0,0));
	ball2.setSpecular(0.5);
	ball2.setRoughness(0.5);

	Sphere ball3(Vec3(-3,-2,0),1.5);
	ball3.setColor(Vec3(1.0,1.0,1.0));
	ball3.setSpecular(1.00);
	ball3.setRoughness(1.0);

	Plane  wall1(Vec3(0 ,0,5),Vec3(0 ,0,-1));
	Plane  wall2(Vec3(-5,0,0),Vec3(1 ,0, 0));
	Plane  wall3(Vec3(5 ,0,0),Vec3(-1,0, 0));
	Plane  wall4(Vec3(0 ,5,0),Vec3(0 ,-1, 0));
	Plane  wall5(Vec3(0 ,-5,0),Vec3(0,1, 0));

	wall1.setColor(Vec3(1,0,0));
	wall2.setColor(Vec3(0,1,0));
	wall3.setColor(Vec3(0,0,1));
	wall4.setColor(Vec3(1,1,0));
	wall5.setColor(Vec3(0,1,1));


	char * line = NULL;
	size_t len  = 0;
	ssize_t read;
	FILE* fp = fopen("cube.obj","r");
	float v1,v2,v3;
	int i1,i2,i3;
	Vec3 displace(2,-2,-2);
	vector<Vec3> vecs;
	vector<Triangle> tris;
	while(( read = getline(&line, &len, fp)) != -1){
		if(sscanf(line, "v %f %f %f",&v1,&v2,&v3)){
			printf("%s",line);
			vecs.push_back(Vec3(v1,v2,v3) + displace);
		}
		//f 5/1/1 3/2/1 1/3/1
		if(sscanf(line, "f %i/%*i/%*i %i/%*i/%*i %i/%*i/%*i",&i1,&i2,&i3)){
			Triangle tri(vecs[i1-1],vecs[i2-1],vecs[i3-1]);
			printf("%s    %i %i %i\n",line,i1,i2,i3);
			tri.setColor(Vec3(0.476990,0.319510,0.288094));
			//0.476990 0.319510 0.288094
			tri.setSpecular(0.20);
			tri.setRoughness(0.90);
			tris.push_back(tri);
		}	
	}

	Triangle tri2(Vec3(0,0,-4),Vec3(-2,-2,-4),Vec3(-2,0,-4));
	tri2.setColor(Vec3(1,0,1));
	tri2.setSpecular(0.00);
	tri2.setRoughness(1.0);

	testscene.addPrimitive(&ball1);
	testscene.addPrimitive(&ball2);
	testscene.addPrimitive(&ball3);
	testscene.addPrimitive(&wall1);
	testscene.addPrimitive(&wall2);
	testscene.addPrimitive(&wall3);
	testscene.addPrimitive(&wall4);
	testscene.addPrimitive(&wall5);
	//testscene.addPrimitive(&tri2);
	for(unsigned int tdx=0; tdx<tris.size(); tdx++){
		testscene.addPrimitive(&tris[tdx]);
	}

	//testscene.addPrimitive(&tri);

	testscene.print();

	int idx = 0;
	int cores = 6;
	char str[10];
	testscene.setRays(Vec3(0,0,-10),Vec3(0,0.1,-1),&plot);
	testscene.setLight(Vec3(0,0,-5));
	for(float y=1;0<=y;y-=0.20){

		ball3.setRoughness(y);

		sprintf(str,"%02d.ppm",idx);

		testscene.render(cores,&plot);
		plot.save(str);

		idx++;
	}
	/*
	for(float y=0;y<4;y+=0.1){
		testscene.setRays(Vec3(y,y,-10),Vec3(0,0.1,-1));
				
		sprintf(str,"%02d.ppm",idx);

		testscene.render(str);
		idx++;
	}
	*/
    return 0;
}
