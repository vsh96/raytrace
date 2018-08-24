#pragma once
#include "vec3.hpp"

class ray
{
public:
    ray(){}
    ray(const vec3& o, const vec3& d){ origin = o; direction = d;}
    vec3 point(float t) const{ return origin + direction*t; }

    vec3 origin;
    vec3 direction;
};