#include <fstream>
#include <float.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include "ray.hpp"
#include "sphere.hpp"
#include "scene.hpp"
#include "camera.hpp"



float hit_sphere(const glm::vec3& pos, float rad, const ray& r)
{
    glm::vec3 oc = r.origin - pos;
    float a = glm::dot(r.direction, r.direction);
    float b = glm::dot(oc, r.direction)*2.0f;
    float c = glm::dot(oc, oc) - rad*rad;
    float d = b*b - 4*a*c;
    if(d < 0) return -1;
    return (-b-sqrt(d))/(2*a);
}

glm::vec3 color(const ray& r, body *b, int depth = 0)
{
    hit h;
    if(b->trace(r, FLT_MAX, h))
    {
        ray ref;
        glm::vec3 atten;
        if(depth<50 && h.matptr->scatter(r, h, atten, ref))
        {
            return atten*color(ref, b, depth+1);
        }else
            return glm::vec3(0,0,0);
        //return (h.normal+vec3(1.0f, 1.0f, 1.0f))*0.5f;
        //return color(ray(h.p, normalize(h.normal+sphereRand())), b)*0.5;
    }
    
    float t = (r.direction.y + 1.0f)*0.5f;
    return glm::vec3(1.0f, 1.0f, 1.0f)*(1.0f - t)+glm::vec3(0.5f, 0.7f, 1.0f)*t;
}

int main()
{
    int IMG_WIDTH = 400;
    int IMG_HEIGHT = 200;
    int SAMPLE_COUNT = 100;

    std::ofstream image("result.ppm");

    image << "P3\n" << IMG_WIDTH << " " << IMG_HEIGHT << "\n255\n";

    glm::vec3 corner(-2.0f, -1.0f, -1.0f);
    glm::vec3 horiz(4.0f, 0.0f, 0.0f);
    glm::vec3 vert(0.0f, 2.0f, 0.0f);
    glm::vec3 origin(0.0f, 0.0f, 0.0f);

    body *list[5];
    list[0] = new sphere(glm::vec3(0.0, 0.0, -1.0), 0.5, new lambertian(glm::vec3(0.8, 0.3, 0.3)));
    list[1] = new sphere(glm::vec3(0.0, -100.5, -1.0), 100.0, new lambertian(glm::vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(glm::vec3(1.0, 0.0, -1.0), 0.5, new metal(glm::vec3(0.8, 0.6, 0.2), 0.3));
    list[3] = new sphere(glm::vec3(-1.0, 0.0, -1.0), 0.5, new dielectric(1.5));
    list[4] = new sphere(glm::vec3(-1.0, 0.0, -1.0), -0.45, new dielectric(1.5));
    body *world = new scene(list, 5);
    camera cam;

    for( int j = IMG_HEIGHT-1; j>=0; j-- )
    for( int i = 0; i<IMG_WIDTH; i++ )
    {
        glm::vec3 col(0,0,0);
        for( int k = 0; k<SAMPLE_COUNT; k++ )
        {
            float u = ( i + drand48() - 0.5 )/float(IMG_WIDTH);
            float v = ( j  + drand48() - 0.5  )/float(IMG_HEIGHT);

            ray r = cam.getRay(u, v);
            col = col + color(r, world);
        }

        col = col/float(SAMPLE_COUNT);
        image << int(sqrt(col[0])*255.99f) << " " << int(sqrt(col[1])*255.99f) << " " << int(sqrt(col[2])*255.99f) << "\n";
    }


    return 0;
}
