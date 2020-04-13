#include <iostream>
#include "src/Vec3.h"
using namespace std;

int main() {

	Vec3 testvec(3,4,5);

    cout << "Hello World!\n";

	printf("Testvec x, y, z = %f, %f, %f\n", testvec.x, testvec.y, testvec.z);
	printf("Testvec length  = %f\n"        , testvec.length());

	Vec3 normvec = testvec.norm();

	printf("Normvec x, y, z = %f, %f, %f\n", normvec.x, normvec.y, normvec.z);
	printf("Normvec length  = %f\n"        , normvec.length());


    return 0;
}
