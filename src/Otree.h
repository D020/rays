#ifndef __Otree_h_
#define __Otree_h_

#include "Vec3.h"

/*
struct node
{
    void* data;
    node* nodes[8];
};
*/

class Otree{
public:
    Otree();
    Otree(Otree* root, Vec3 min, Vec3 max);
    void divide();

    Vec3 getMin();
    Vec3 getMax();
    Otree* getNode(int ndx);

    void setData(void* data);
    void* getData();

private:
    //node* root;
    Otree* root;
    Otree* nodes[8];
    void* data;
    Vec3 min;
    Vec3 max;
};

#endif