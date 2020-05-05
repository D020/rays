#include "Obj.h"
#include <stdio.h>
#include <limits>
#include <math.h>
#include <algorithm>

int Obj::assignTris(Otree* subTree, vector<Triangle*>* subTris){
	vector<Triangle*>* newTris = new vector<Triangle*>;

	int within = 0;

	for(auto& subTri : *subTris){

		//if(subTri->inBox(subTree->getMin(),subTree->getMax())){
		if(subTri->triangleIntersectBox(subTree->getMin(),subTree->getMax()) || subTri->inBox(subTree->getMin(),subTree->getMax())){
			newTris->push_back(subTri);
			within++;
		}
	}

	subTree->setData((void*) newTris);
	return within;
}

void Obj::distributeTris(Otree* subTree, vector<Triangle*>* subTris){
	int within = assignTris(subTree,subTris);
	if(within <= 32){//If the assignment contains less than 32 we are done
		return;
	}
	//Otherwise continue dividing and distributing!
	subTree->divide();
	for(int ndx=0;ndx<8;ndx++){
		Otree* nodeTree = subTree->getNode(ndx);
		vector<Triangle*>* nodeTris = (vector<Triangle*>*)subTree->getData();

		distributeTris(nodeTree,nodeTris);
	}

}

Obj::Obj(char* path, Vec3 displace, int sub){

	float subd = sub;
	
	char * line = NULL;
	size_t len  = 0;
	ssize_t read;
	FILE* fp = fopen(path,"r");
	float v1,v2,v3;
	int i1,i2,i3;
	vector<Vec3> vecs;

	float min_x = std::numeric_limits<float>::infinity();
	float max_x = -std::numeric_limits<float>::infinity();
	float min_y = std::numeric_limits<float>::infinity();
	float max_y = -std::numeric_limits<float>::infinity();
	float min_z = std::numeric_limits<float>::infinity();
	float max_z = -std::numeric_limits<float>::infinity();

	while(( read = getline(&line, &len, fp)) != -1){
		if(sscanf(line, "v %f %f %f",&v1,&v2,&v3)){
			Vec3 insert = Vec3(v1,v2,v3) + displace;
			vecs.push_back(insert);
	
			
			if(insert.getX()<min_x)
				min_x = insert.getX();
			if(insert.getY()<min_y)
				min_y = insert.getY();
			if(insert.getZ()<min_z)
				min_z = insert.getZ();

			if(max_x<insert.getX())
				max_x = insert.getX();
			if(max_y<insert.getY())
				max_y = insert.getY();
			if(max_z<insert.getZ())
				max_z = insert.getZ();


		}
		/*
		if(sscanf(line, "f %i/%*i/%*i %i/%*i/%*i %i/%*i/%*i",&i1,&i2,&i3)){
			Triangle tri(vecs[i1-1],vecs[i2-1],vecs[i3-1]);
			tri.setColor(Vec3(0.476990,0.319510,0.288094));
			tri.setSpecular(0.20);
			tri.setRoughness(0.90);
			tris.push_back(tri);
		}	
		*/
		if(sscanf(line, "f %i/%*i/%*i %i/%*i/%*i %i/%*i/%*i",&i1,&i2,&i3)){
			Triangle* tri = new Triangle(vecs[i1-1],vecs[i2-1],vecs[i3-1]);
			tri->setColor(Vec3(0.476990,0.319510,0.288094));
			tri->setSpecular(0.20);
			tri->setRoughness(0.90);
			tris.push_back(tri);
		}	
	}

	minG = Vec3(min_x,min_y,min_z);
	maxG = Vec3(max_x,max_y,max_z);

	tree = Otree(0,minG,maxG);

	distributeTris(&tree,&tris);

	printf("Done distributing across Octree\n");

	/*
	float x_range = maxG.getX() - minG.getX();
	float y_range = maxG.getY() - minG.getY();
	float z_range = maxG.getZ() - minG.getZ();
	int cubes = 0;
	for(float xsub=0; xsub<subd; xsub++){
		float min_x = (xsub/subd)*x_range + minG.getX();
		float max_x = ((xsub+1)/subd)*x_range + minG.getX();
		for(float ysub=0; ysub<subd; ysub++){
			float min_y = (ysub/subd)*y_range + minG.getY();
			float max_y = ((ysub+1)/subd)*y_range + minG.getY();
			for(float zsub=0; zsub<subd; zsub++){
				float min_z = (zsub/subd)*z_range + minG.getZ();
				float max_z = ((zsub+1)/subd)*z_range + minG.getZ();
				
				Vec3 pushMin = Vec3(min_x,min_y,min_z);
				Vec3 pushMax = Vec3(max_x,max_y,max_z);

				minSub.push_back(pushMin);
				maxSub.push_back(pushMax);
				
				vector<int> indices;				

				for(unsigned int tdx=0; tdx<tris.size(); tdx++){

					if(tris[tdx].inBox(pushMin,pushMax))
						indices.push_back(tdx);
				
				}
				indSub.push_back(indices);
				cubes++;
			}
		}
	}
	*/

}

bool Obj::boxIntersect(Ray ray, Vec3 min, Vec3 max) { 
//Almost direct copy of:
//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
    float tmin = (min.getX() - ray.getOrg().getX()) / ray.getDir().getX(); 
    float tmax = (max.getX() - ray.getOrg().getX()) / ray.getDir().getX();
 
    if (tmin > tmax) swap(tmin, tmax); 
 
    float tymin = (min.getY() - ray.getOrg().getY()) / ray.getDir().getY(); 
    float tymax = (max.getY() - ray.getOrg().getY()) / ray.getDir().getY(); 
 
    if (tymin > tymax) swap(tymin, tymax); 
 
    if ((tmin > tymax) || (tymin > tmax)) 
        return false; 
 
    if (tymin > tmin) 
        tmin = tymin; 
 
    if (tymax < tmax) 
        tmax = tymax; 
 
    float tzmin = (min.getZ() - ray.getOrg().getZ()) / ray.getDir().getZ(); 
    float tzmax = (max.getZ() - ray.getOrg().getZ()) / ray.getDir().getZ(); 
 
    if (tzmin > tzmax) swap(tzmin, tzmax); 
 
    if ((tmin > tzmax) || (tzmin > tmax)) 
        return false; 
 
    if (tzmin > tmin) 
        tmin = tzmin; 
 
    if (tzmax < tmax) 
        tmax = tzmax; 
 
    return true; 
} 

vector<Triangle*> Obj::recursiveIntersect(Ray ray, Otree* subTree, vector<Triangle*> soFar){

	if(subTree->getNode(0)==0){
		vector<Triangle*>* tmp = (vector<Triangle*>*)subTree->getData();
		soFar.insert( soFar.end(), tmp->begin(), tmp->end() );
		return soFar;
	}
	for(int ndx=0; ndx<8; ndx++){
		if(boxIntersect(ray, subTree->getNode(ndx)->getMin(), subTree->getNode(ndx)->getMax())){
			soFar = recursiveIntersect(ray,subTree->getNode(ndx), soFar);
		}
	}
	return soFar;
}

Collision Obj::intersect(Ray ray){

	Collision colMin;
	colMin.normal   = -1.0*ray.getDir().norm();
	colMin.distance = std::numeric_limits<float>::infinity();

	if(!boxIntersect(ray, minG, maxG))
		return colMin;
	
	vector<Triangle*> relevantTris;
	relevantTris = recursiveIntersect(ray,&tree,relevantTris);

	for(auto& tri : relevantTris){
		Collision curCol = tri->intersect(ray);
		if (curCol.distance < colMin.distance && 0 < curCol.distance){
			colMin.distance = curCol.distance;
			colMin  = curCol;
		}
	}

	return colMin;
	/*
	for(unsigned int bdx=0;bdx<minSub.size();bdx++){
		if(boxIntersect(ray, minSub[bdx], maxSub[bdx])){
			for(unsigned int pdx=0; pdx<indSub[bdx].size(); pdx++){
				Collision curCol = tris[indSub[bdx][pdx]].intersect(ray);
				if (curCol.distance < colMin.distance && 0 < curCol.distance){
					colMin.distance = curCol.distance;
					colMin  = curCol;
				}
			}
		}
	}
	*/

/*
	
	for(unsigned int pdx=0; pdx<tris.size(); pdx++){
		Collision curCol = tris[pdx].intersect(ray);
		if (curCol.distance < colMin.distance && 0 < curCol.distance){
			colMin.distance = curCol.distance;
			colMin  = curCol;
		}
	}
*/
	//return colMin;
}

void Obj::print(){
	printf("Object with displacement:\n");
	this->displace.print(false);
}
