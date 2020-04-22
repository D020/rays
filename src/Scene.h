#ifndef __Scene_h_
#define __Scene_h_

#include "Primitive.h"
#include "Plot.h"
#include "Vec3.h"
#include "Ray.h"

struct SceneCollision {
    Vec3 normal;	//Normal to the point that was hit.
	Vec3 position;  //Position in the scene where Ray ended.
    float distance; 
	int hitIndex;	//Index into primitives indicating which object was hit first.
};

class Scene {
public:
	Scene();
	Scene(int width, int height);
	void addPrimitive(Primitive* prim);
	void setRays(Vec3 org, Vec3 dir);
	SceneCollision intersect(Ray ray, int ignore);
	void render(const char* path);
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
