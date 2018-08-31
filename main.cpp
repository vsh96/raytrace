#include <fstream>
#include <float.h>
#include <stdlib.h>
#include "vec3.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "scene.hpp"
#include "camera.hpp"

vec3 sphereRand()
{
    vec3 p;
    do{
        p = vec3(drand48(),drand48(),drand48())*2 - vec3(1,1,1);
    }while(dot(p,p)<1.0f);
    return p;
}

float hit_sphere(const vec3& pos, float rad, const ray& r)
{
    vec3 oc = r.origin - pos;
    float a = dot(r.direction, r.direction);
    float b = dot(oc, r.direction)*2.0f;
    float c = dot(oc, oc) - rad*rad;
    float d = b*b - 4*a*c;
    if(d < 0) return -1;
    return (-b-sqrt(d))/(2*a);
}

vec3 color(const ray& r, body *b)
{
    hit h;
    if(b->trace(r, FLT_MAX, h))
    {
        //return (h.normal+vec3(1.0f, 1.0f, 1.0f))*0.5f;
        return color(ray(h.p, normalize(h.normal+sphereRand())), b)*0.5;
    }
    
    float t = (r.direction.y() + 1.0f)*0.5f;
    return vec3(1.0f, 1.0f, 1.0f)*(1.0f - t)+vec3(0.5f, 0.7f, 1.0f)*t;
}

int main()
{
    int IMG_WIDTH = 400;
    int IMG_HEIGHT = 200;
    int SAMPLE_COUNT = 100;

    std::ofstream image("result.ppm");

    image << "P3\n" << IMG_WIDTH << " " << IMG_HEIGHT << "\n255\n";

    vec3 corner(-2.0f, -1.0f, -1.0f);
    vec3 horiz(4.0f, 0.0f, 0.0f);
    vec3 vert(0.0f, 2.0f, 0.0f);
    vec3 origin(0.0f, 0.0f, 0.0f);

    body *list[2];
    list[0] = new sphere(vec3(0.0, 0.0, -1.0), 0.5);
    list[1] = new sphere(vec3(0.0, -100.5, -1.0), 100.0);
    body *world = new scene(list, 2);
    camera cam;

    for( int j = IMG_HEIGHT-1; j>=0; j-- )
    for( int i = 0; i<IMG_WIDTH; i++ )
    {
        vec3 col(0,0,0);
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
