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

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted)
{
    vec3 uv = normalize(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if(discriminant > 0)
    {
        refracted = (uv-n*dt)*ni_over_nt - n*sqrt(discriminant);
        return true;
    }
    else
        return false;
}

float schlick(float cosine, float ref_idx)
{
    float r0 = (1-ref_idx)/(1+ref_idx);
    r0=r0*r0;
    return r0+(1-r0)*pow((1-cosine), 5);
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

class dielectric : public material {
    public:
        dielectric(float ri) : ref_idx(ri) {}
        virtual bool scatter(const ray in, const hit& h, vec3& atten, ray& ref) const 
        {
            vec3 outward_normal;
            vec3 reflected = reflect(in.direction, h.normal);
            float ni_over_nt;
            atten = vec3(1.0, 1.0, 1.0);
            vec3 refracted;
            float reflect_prob;
            float cosine;
            if(dot(in.direction, h.normal) > 0)
            {
                outward_normal = -h.normal;
                ni_over_nt = ref_idx;
                cosine = ref_idx*dot(in.direction, h.normal)/length(in.direction);
            }
            else
            {
                outward_normal = h.normal;
                ni_over_nt = 1.0/ref_idx;
                cosine = -dot(in.direction, h.normal)/length(in.direction);
            }
            if(refract(in.direction, outward_normal, ni_over_nt, refracted))
            {
                reflect_prob = schlick(cosine, ref_idx);
            }
            else
            {
                ref = ray(h.p, reflected);
                reflect_prob = 1.0;
            }

            if(drand48() < reflect_prob)
            {
                ref = ray(h.p, reflected);
            }
            else
            {
                ref = ray(h.p, refracted);
            }
            return true;
        }
        float ref_idx;
};