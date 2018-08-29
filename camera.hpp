#pragma once
#include "ray.hpp"

class camera
{
public:
    camera()
    {
        corner = vec3(-2.0f, -1.0f, -1.0f);
        horiz = vec3(4.0f, 0.0f, 0.0f);
        vert = vec3(0.0f, 2.0f, 0.0f);
        origin = vec3(0.0f, 0.0f, 0.0f);
    }
    ray getRay(float u, float v)
    {return ray(origin, normalize(corner + horiz*u + vert*v));}

    vec3 corner;
    vec3 horiz;
    vec3 vert;
    vec3 origin;
};