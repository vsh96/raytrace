#pragma once
#include "body.hpp"

class sphere: public body
{
public:
    sphere(){}
    sphere(vec3 p, float r): pos(p), rad(r) {}
    virtual bool trace(const ray& r, float max, hit& h) const;
    vec3 pos;
    float rad;
};

bool sphere::trace(const ray& r, float max, hit& h) const
{
    vec3 oc = r.origin - pos;
    float a = dot(r.direction, r.direction);
    float b = dot(oc, r.direction);
    float c = dot(oc, oc) - rad*rad;
    float d = b*b - a*c;
    if(d > 0)
    {
        float temp = (-b-sqrt(d))/a;
        if(temp<max && temp>0)
        {
            h.t = temp;
            h.p = r.point(temp);
            h.normal = (h.p - pos)/rad;
            return true;
        }
        temp = (-b+sqrt(d))/a;
        if(temp<max && temp>0)
        {
            h.t = temp;
            h.p = r.point(temp);
            h.normal = (h.p - pos)/rad;
            return true;
        }
    }
    return false;
}