#pragma once
#include "body.hpp"

class sphere: public body
{
public:
    sphere(){}
    sphere(glm::vec3 p, float r, material* m): pos(p), rad(r), matptr(m) {}
    virtual bool trace(const ray& r, float max, hit& h) const;
    material* matptr;
    glm::vec3 pos;
    float rad;
};

bool sphere::trace(const ray& r, float max, hit& h) const
{
    glm::vec3 oc = r.origin - pos;
    float a = dot(r.direction, r.direction);
    float b = glm::dot(oc, r.direction);
    float c = glm::dot(oc, oc) - rad*rad;
    float d = b*b - a*c;
    if(d > 0)
    {
        float temp = (-b-sqrt(d))/a;
        if(temp<max && temp>0.001)
        {
            h.t = temp;
            h.p = r.point(temp);
            h.normal = (h.p - pos)/rad;
            h.matptr = matptr;
            return true;
        }
        temp = (-b+sqrt(d))/a;
        if(temp<max && temp>0.001)
        {
            h.t = temp;
            h.p = r.point(temp);
            h.normal = (h.p - pos)/rad;
            h.matptr = matptr;
            return true;
        }
    }
    return false;
}