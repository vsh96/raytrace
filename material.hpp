#pragma once
#include <stdlib.h>
#include "ray.hpp"

class material;

struct hit
{
    float t;
    glm::vec3 p;
    glm::vec3 normal;
    material* matptr;
};

glm::vec3 sphereRand()
{
    glm::vec3 p;
    do{
        p = glm::vec3(drand48(),drand48(),drand48())*2.f - glm::vec3(1,1,1);
    }while(glm::dot(p,p)>1.0f);
    return p;
}

glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n)
{
    return v - n*glm::dot(v,n)*2.f;
}

bool refract(const glm::vec3& v, const glm::vec3& n, float ni_over_nt, glm::vec3& refracted)
{
    glm::vec3 uv = glm::normalize(v);
    float dt = glm::dot(uv, n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if(discriminant > 0)
    {
        refracted = (uv-n*dt)*ni_over_nt - n*(float)sqrt(discriminant);
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
    virtual bool scatter(const ray in, const hit& h, glm::vec3& atten, ray& ref) const = 0;
};

class lambertian: public material
{
public:
    lambertian(const glm::vec3 a): albedo(a) {}
    virtual bool scatter(const ray in, const hit& h, glm::vec3& atten, ray& ref) const
    {
        ref = ray(h.p, normalize(h.normal+sphereRand()));
        atten = albedo;
        return true;
    }
    glm::vec3 albedo;
};

class metal: public material
{
public:
    metal(const glm::vec3 a, float r): albedo(a) {if(r>1)roughness=1.0; else roughness = r;}
    virtual bool scatter(const ray in, const hit& h, glm::vec3& atten, ray& ref) const
    {
        ref = ray(h.p, reflect(in.direction, h.normal)+sphereRand()*roughness);
        atten = albedo;
        return (glm::dot(ref.direction, h.normal) > 0);
    }
    float roughness;
    glm::vec3 albedo;
};

class dielectric : public material {
    public:
        dielectric(float ri) : ref_idx(ri) {}
        virtual bool scatter(const ray in, const hit& h, glm::vec3& atten, ray& ref) const 
        {
            glm::vec3 outward_normal;
            glm::vec3 reflected = reflect(in.direction, h.normal);
            float ni_over_nt;
            atten = glm::vec3(1.0, 1.0, 1.0);
            glm::vec3 refracted;
            float reflect_prob;
            float cosine;
            if(glm::dot(in.direction, h.normal) > 0)
            {
                outward_normal = -h.normal;
                ni_over_nt = ref_idx;
                cosine = ref_idx*glm::dot(in.direction, h.normal)/length(in.direction);
            }
            else
            {
                outward_normal = h.normal;
                ni_over_nt = 1.0/ref_idx;
                cosine = -glm::dot(in.direction, h.normal)/length(in.direction);
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