#pragma once
#include "ray.hpp"

struct hit
{
    float t;
    vec3 p;
    vec3 normal;
};

class body
{
public:
    virtual bool trace(const ray& r, float max, hit& h) const = 0;
};