#include <iostream>
#include <stdio.h>
#include "src/Vec3.h"
#include "src/Plot.h"
#include "src/Ray.h"
#include "src/Primitive.h"
#include "src/Sphere.h"
#include "src/Scene.h"
#include <math.h>
#include <algorithm>
using namespace std;



int main() {

	printf("\n\n\n\n\n");

	Sphere ball1(Vec3(0,0,0),2);
	Sphere ball2(Vec3(3,1,0),2.5);

	Scene testscene(1920,1080);
	testscene.addPrimitive(&ball1);
	testscene.addPrimitive(&ball2);

	testscene.print();
	testscene.setRays(Vec3(0,0,-5),Vec3(0,0,1));
	testscene.render("test.ppm");

    return 0;
}
