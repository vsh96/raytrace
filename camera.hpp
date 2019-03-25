#pragma once
#include "ray.hpp"

class camera
{
public:
    camera()
    {
        corner = glm::vec3(-2.0f, -1.0f, -1.0f);
        horiz = glm::vec3(4.0f, 0.0f, 0.0f);
        vert = glm::vec3(0.0f, 2.0f, 0.0f);
        origin = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    ray getRay(float u, float v)
    {return ray(origin, normalize(corner + horiz*u + vert*v));}

    glm::vec3 corner;
    glm::vec3 horiz;
    glm::vec3 vert;
    glm::vec3 origin;
};