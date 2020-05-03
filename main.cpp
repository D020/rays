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
#include "src/Obj.h"
#include "src/Scene.h"
#include <math.h>
#include <algorithm>
#include <thread>
#include <chrono> 
using namespace std;
using namespace std::chrono;

int main() {
	//Benchmark size /8
	Plot plot(1920/2,1080/2);
	

	Scene testscene;

	printf("\n\n\n\n\n");

	Sphere ball1(Vec3(0,0,0),2);	
	ball1.setColor(Vec3(0,0,1.0));
	ball1.setReflectance(0.7);	
	ball1.setRoughness(0.05);	

	Sphere ball2(Vec3(3,1,0),2.5);
	ball2.setColor(Vec3(1.0,0,0));
	ball2.setReflectance(0.8);
	ball2.setRoughness(0.5);

	Sphere ball3(Vec3(-3,-2,0),1.5);
	ball3.setColor(Vec3(1.0,1.0,1.0));
	ball3.setReflectance(1.0);
	ball3.setRoughness(0.0);

	Plane  wall1(Vec3(0 ,0,5),Vec3(0 ,0,-1));
	Plane  wall2(Vec3(-5,0,0),Vec3(1 ,0, 0));
	Plane  wall3(Vec3(5 ,0,0),Vec3(-1,0, 0));
	Plane  wall4(Vec3(0 ,5,0),Vec3(0 ,-1, 0));
	Plane  floor(Vec3(0 ,-5,0),Vec3(0,1, 0));

	wall1.setColor(Vec3(1,0,0));
	wall2.setColor(Vec3(0,1,0));
	wall3.setColor(Vec3(0,0,1));
	wall4.setColor(Vec3(1,1,0));
	floor.setColor(Vec3(0,1,1));


	//Obj man("objects/man_sub.obj", Vec3(2,-2,-2), 4);
	Obj man("objects/man_sub2.obj", Vec3(2,-2,-2), 4);
	//Obj man("objects/cube.obj", Vec3(2,-2,-2), 4);
	man.setColor(Vec3(0.476990,0.319510,0.288094));
	man.setReflectance(0.50);
	man.setRoughness(0.10);

	testscene.addPrimitive(&ball1);
	testscene.addPrimitive(&ball2);
	testscene.addPrimitive(&ball3);
	testscene.addPrimitive(&wall1);
	testscene.addPrimitive(&wall2);
	testscene.addPrimitive(&wall3);
	testscene.addPrimitive(&wall4);
	testscene.addPrimitive(&floor);
	testscene.addPrimitive(&man);

	testscene.print();

	int idx = 0;
	int cores = 24;
	char str[10];

	testscene.setRays(Vec3(0,0,-10),Vec3(0,0.1,-1),&plot);
	testscene.addPointLight(Vec3(0,0,0),100);
	testscene.addPointLight(Vec3(2,4,-5),100);
	for(float y=0;y<=6;y+=0.2){

		testscene.moveLight(0,Vec3(-3+y,0,-5));
		//ball3.setRoughness(y);
		sprintf(str,"%02d.ppm",idx);

		auto start = high_resolution_clock::now();
		testscene.render(cores,32,&plot);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);

		printf("Time: %i\n",int(duration.count()/(1000)));

		plot.save(str);	

		return 0;

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
