#ifndef __Obj_h_
#define __Obj_h_

#include "Primitive.h"
#include "Triangle.h"
#include <vector>
using namespace std;

class Obj : public Primitive {
public:
	Obj(char* path, Vec3 displace);
	virtual Collision intersect(Ray ray);
	bool boxIntersect(Ray ray);
	virtual void print();
private:
	Vec3 displace;
	Vec3 min;
	Vec3 max;
	vector<Triangle> tris;

	vector<Vec3> minSub;
	vector<Vec3> maxSub;
	vector< vector<int> > indSub;
};

#endif
