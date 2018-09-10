#pragma once
#include <stdlib.h>
#include "ray.hpp"

class material;

struct hit
{
    float t;
    vec3 p;
    vec3 normal;
    material* matptr;
};

vec3 sphereRand()
{
    vec3 p;
    do{
        p = vec3(drand48(),drand48(),drand48())*2 - vec3(1,1,1);
    }while(dot(p,p)>1.0f);
    return p;
}

vec3 reflect(const vec3& v, const vec3& n)
{
    return v - n*dot(v,n)*2;
}

class material
{
public:
    virtual bool scatter(const ray in, const hit& h, vec3& atten, ray& ref) const = 0;
};

class lambertian: public material
{
public:
    lambertian(const vec3 a): albedo(a) {}
    virtual bool scatter(const ray in, const hit& h, vec3& atten, ray& ref) const
    {
        ref = ray(h.p, normalize(h.normal+sphereRand()));
        atten = albedo;
        return true;
    }
    vec3 albedo;
};

class metal: public material
{
public:
    metal(const vec3 a, float r): albedo(a) {if(r>1)roughness=1.0; else roughness = r;}
    virtual bool scatter(const ray in, const hit& h, vec3& atten, ray& ref) const
    {
        ref = ray(h.p, reflect(in.direction, h.normal)+sphereRand()*roughness);
        atten = albedo;
        return (dot(ref.direction, h.normal) > 0);
    }
    float roughness;
    vec3 albedo;
};