#ifndef __Scene_h_
#define __Scene_h_

#include "Primitive.h"
#include "Plot.h"
#include "Vec3.h"
#include "Ray.h"
class Scene {
public:
	Scene();
	Scene(int width, int height);
	void addPrimitive(Primitive* prim);
	void setRays(Vec3 org, Vec3 dir);
	void print();
private:
	int         noPrimitives;
	Primitive** primitives;
	Plot        plot;
	Ray*		rays;
	
	float horizontalFOVradians;
	float   verticalFOVradians;

};

#endif
