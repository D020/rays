#ifndef __Obj_h_
#define __Obj_h_

#include "Primitive.h"
#include "Triangle.h"
#include <vector>
#include "Otree.h"
using namespace std;

class Obj : public Primitive {
public:
	Obj(char* path, Vec3 displace, int sub);
	virtual Collision intersect(Ray ray);
	bool boxIntersect(Ray ray, Vec3 min, Vec3 max);
	virtual void print();
private:
	Vec3 displace;
	Vec3 minG;
	Vec3 maxG;
	//vector<Triangle> tris;
	vector<Triangle*> tris;

	vector<Vec3> minSub;
	vector<Vec3> maxSub;
	vector< vector<int> > indSub;

	Otree tree;

	int assignTris(Otree* subTree, vector<Triangle*>* subTris);
	void distributeTris(Otree* subTree, vector<Triangle*>* subTris);

	vector<Triangle*> recursiveIntersect(Ray ray, Otree* subTree, vector<Triangle*> soFar);
};

#endif
