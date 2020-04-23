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
#include "src/Scene.h"
#include <math.h>
#include <algorithm>
using namespace std;



int main() {

	printf("\n\n\n\n\n");

	Sphere ball1(Vec3(0 ,0,0),2);	
	ball1.setColor(Vec3(0,0,1.0));
	ball1.setSpecular(0.1);	

	Sphere ball2(Vec3(3 ,1,0),2.5);
	ball2.setColor(Vec3(1.0,0,0));
	ball2.setSpecular(0.70);

	Sphere ball3(Vec3(-3,-2,0),1.5);
	ball3.setColor(Vec3(1.0,1.0,1.0));
	ball3.setSpecular(1.00);

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

	//Scene testscene(534/3,300/3);
	Scene testscene(1280,720);
	testscene.addPrimitive(&ball1);
	testscene.addPrimitive(&ball2);
	testscene.addPrimitive(&ball3);
	testscene.addPrimitive(&wall1);
	testscene.addPrimitive(&wall2);
	testscene.addPrimitive(&wall3);
	testscene.addPrimitive(&wall4);
	testscene.addPrimitive(&wall5);

	testscene.print();

	int idx = 0;
	char str[10];
	for(float y=4;0<y;y-=0.1){
		testscene.setRays(Vec3(y,y,-10),Vec3(0,0.1,-1));
				
		sprintf(str,"%02d.ppm",idx);

		testscene.render(str);

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
