#include <fstream>
#include <float.h>
#include "vec3.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "scene.hpp"

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
        return (h.normal+vec3(1.0f, 1.0f, 1.0f))*0.5f;
    }
    
    float t = (r.direction.y() + 1.0f)*0.5f;
    return vec3(1.0f, 1.0f, 1.0f)*(1.0f - t)+vec3(0.5f, 0.7f, 1.0f)*t;
}

int main()
{
    int IMG_WIDTH = 400;
    int IMG_HEIGHT = 200;

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
    //body *world = new sphere(vec3(0.0, 0.0, -1.0), 0.5);

    for( int j = IMG_HEIGHT-1; j>=0; j-- )
    for( int i = 0; i<IMG_WIDTH; i++ )
    {
        float u = float( i )/float(IMG_WIDTH);
        float v = float( j )/float(IMG_HEIGHT);

        ray r(origin, normalize(corner + horiz*u + vert*v));
        vec3 col = color(r, world);

        float b = 0.2f;
        image << int(col[0]*255.99f) << " " << int(col[1]*255.99f) << " " << int(col[2]*255.99f) << "\n";
    }


    return 0;
}
