#include "Otree.h"
#include <stdio.h>
#include <string.h>

Otree::Otree(){
    this->root = 0;
    this->min = Vec3(0,0,0);
    this->max = Vec3(0,0,0);
    memset((void*)nodes,0,8*sizeof(Otree*));
}

Otree::Otree(Otree* root, Vec3 min, Vec3 max){
    this->root = root;
    this->min = min;
    this->max = max;
    memset((void*)nodes,0,8*sizeof(Otree*));
}

void Otree::setData(void* data){
    this->data = data;
}

void* Otree::getData(){
    return data;
}

Vec3 Otree::getMin(){
    return min;
}
Vec3 Otree::getMax(){
    return max;
}

Otree* Otree::getNode(int ndx){
    return nodes[ndx];
}

void Otree::divide(){
    int ndx = 0;
    float xRange = max.getX()-min.getX();
    float yRange = max.getY()-min.getY();
    float zRange = max.getZ()-min.getZ();
    for(float xdx=0;xdx<=1;xdx++){
        float xmin = min.getX() + (xdx)   * xRange/2;
        float xmax = xmin       + (xdx+1) * xRange/2;
        for(float ydx=0;ydx<=1;ydx++){
            float ymin = min.getY() + (ydx)   * yRange/2;
            float ymax = ymin       + (ydx+1) * yRange/2;
            for(float zdx=0;zdx<=1;zdx++){
                float zmin = min.getZ() + (zdx)   * zRange/2;
                float zmax = zmin       + (zdx+1) * zRange/2;
                //printf("node[%i]: xmin %f xmax %f ymin %f ymax %f zmin %f zmax %f\n",ndx,xmin,xmax,ymin,ymax,zmin,zmax);

                nodes[ndx] = new Otree(this,Vec3(xmin,ymin,zmin),Vec3(xmax,ymax,zmax));

                ndx++;
            }
        }
    }


}