#include <iostream>
#include <stdio.h>
#include "src/Vec3.h"
#include "src/Plot.h"

using namespace std;

int main() {

	Vec3 testvec(3,4,5);

    cout << "Hello World!\n";

	printf("Testvec x, y, z = %f, %f, %f\n", testvec.x, testvec.y, testvec.z);
	printf("Testvec length  = %f\n"        , testvec.length());

	Vec3 normvec = testvec.norm();

	printf("Normvec x, y, z = %f, %f, %f\n", normvec.x, normvec.y, normvec.z);
	printf("Normvec length  = %f\n"        , normvec.length());

	Plot image(1280,720);
	image.plot(1280/2,720/2,255,0,0);
	image.plot(1280-1,720-1,0,255,0);
	image.save("test.ppm");

    return 0;
}
