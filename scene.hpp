#pragma once
#include "body.hpp"

class scene: public body
{
public:
    scene(){}
    scene(body** l, int n){ list = l; size = n; }
    virtual bool trace(const ray& r, float max, hit& h) const;
    body **list;
    int size;
};

bool scene::trace(const ray& r, float max, hit& h) const
{
    hit temp;
    bool hit_anything = false;
    double nearest = max;
    for(int i = 0; i<size; i++)
    {
        if(list[i]->trace(r, nearest, temp))
        {
            hit_anything = true;
            nearest = temp.t;
            h = temp;
        }
    }
    return hit_anything;
}