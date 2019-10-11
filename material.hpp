#pragma once
#include <glm/gtx/norm.hpp>
#include <glm/gtc/random.hpp>
#include "ray.hpp"

class material;

struct hit
{
    float t;
    glm::vec3 p;
    glm::vec3 normal;
    material* matptr;
};

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
        ref = ray(h.p, glm::normalize(h.normal+glm::ballRand(1.0f)));
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
        ref = ray(h.p, glm::reflect(in.direction, h.normal)+glm::ballRand(1.0f)*roughness);
        atten = albedo;
        return true;
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
            glm::vec3 refracted = glm::refract(in.direction, outward_normal, ni_over_nt);
            if(length2(refracted)>0.0)
            {
                reflect_prob = schlick(cosine, ref_idx);
            }
            else
            {
                ref = ray(h.p, reflected);
                return true;
            }

            if(glm::linearRand(0.0, 1.0) < reflect_prob)
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
