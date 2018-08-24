#include <fstream>
#include "vec3.hpp"
#include "ray.hpp"

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

vec3 color(const ray& r)
{
    float t = hit_sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, r);
    if(t>0)
    {
        vec3 n = normalize(r.point(t)-vec3(0.0f, 0.0f, -1.0f));
        return (n+vec3(1.0f, 1.0f, 1.0f))*0.5f;
    }
    
    t = (r.direction.y() + 1.0f)*0.5f;
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

    for( int j = IMG_HEIGHT-1; j>=0; j-- )
    for( int i = 0; i<IMG_WIDTH; i++ )
    {
        float u = float( i )/float(IMG_WIDTH);
        float v = float( j )/float(IMG_HEIGHT);

        ray r(origin, normalize(corner + horiz*u + vert*v));
        vec3 col = color(r);

        float b = 0.2f;
        image << int(col[0]*255.99f) << " " << int(col[1]*255.99f) << " " << int(col[2]*255.99f) << "\n";
    }


    return 0;
}
