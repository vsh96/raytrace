#pragma once
#include <glm/glm.hpp>

class ray
{
public:
    ray(){}
    ray(const glm::vec3& o, const glm::vec3& d){ origin = o; direction = d;}
    glm::vec3 point(float t) const{ return origin + direction*t; }

    glm::vec3 origin;
    glm::vec3 direction;
};