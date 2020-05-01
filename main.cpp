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
	//Plot plot(1920,1080);

	//4,4 with 16
	//23410
	//4,4 with 32
	//23028
	//4,4 with 64
	//25850

	//4,4 with 16 and BB
	//3903
	//4,4 with 32 and BB
	//4183
	//4,4 with 64 and BB
	//5084

	//4,4 with 16 and BB SUB4
	//1090


	//1,1 with 16 and BB
	//62365
	//1,1 with 32 and BB
	//60237
	//1,1 with 64 and BB
	//60121


	//Plot plot(534,300);
	Plot plot(1280,720);
	

	Scene testscene;

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

	Obj man("man_sub2.obj", Vec3(2,-2,-2), 4);
	man.setColor(Vec3(0.476990,0.319510,0.288094));
	man.setSpecular(0.30);
	man.setRoughness(0.90);

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
	testscene.addPrimitive(&man);

	testscene.print();

	int idx = 0;
	int cores = 12;
	char str[10];
	testscene.setRays(Vec3(0,0,-10),Vec3(0,0.1,-1),&plot);
	for(float y=0;y<=6;y+=0.2){

		testscene.setLight(Vec3(-3+y,0,-5));
		//ball3.setRoughness(y);

		sprintf(str,"%02d.ppm",idx);

		auto start = high_resolution_clock::now();
		testscene.render(cores,32,&plot);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);

		printf("Time: %i\n",int(duration.count()/(1000)));

		plot.save(str);	

		//return 0;

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
