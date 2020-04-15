#include <iostream>
#include <stdio.h>
#include "src/Vec3.h"
#include "src/Plot.h"
#include "src/Ray.h"

using namespace std;

int main() {

	Vec3 testvec(3,4,5);
	testvec.print(true);

	Vec3 normvec = testvec.norm();
	normvec.print(true);

	Plot image(1280,720);
	image.plot(1280/2,720/2,255,0,0);
	image.plot(1280-1,720-1,0,255,0);
	//image.save("test.ppm");

	Ray lol(Vec3(1,2,3),Vec3(4,5,6));
	lol.print();
	

	float dotprod = testvec * testvec;
	printf("%f\n",dotprod);

    return 0;
}
