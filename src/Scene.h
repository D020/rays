#ifndef __Scene_h_
#define __Scene_h_

#include "Primitive.h"
#include <stdlib.h>
#include "Plot.h"
#include "Vec3.h"
#include "Ray.h"
#include <vector>
using namespace std;

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
	void setRays(Vec3 org, Vec3 dir, Plot* plot);
	void setLight(Vec3 org);
	SceneCollision intersect(Ray ray, int ignore);

	vector<Ray> SceneTraceBundle(vector<Ray> rays);

	void render(int cores, Plot* plot);
	void renderPart(int ya, int yb, Plot* plot);
	void save(const char* path);
	void print();
private:
	int         noPrimitives;
	Primitive** primitives;
	vector<Ray>	rays;
	Vec3 		light;
	float horizontalFOVradians;
	float   verticalFOVradians;

	int32_t prng();
	float   prng_range(float a, float b);

	int32_t random;

};

#endif
