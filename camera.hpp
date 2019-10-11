#pragma once
#include "ray.hpp"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 lookat2(glm::vec3 pos, glm::vec3 tar)
{
    tar=glm::normalize(tar-pos);
    glm::vec3 tmp = glm::cross(glm::vec3(0.0, 1.0, 0.0), tar);
    return glm::mat4(glm::vec4(tmp, 0), glm::vec4(glm::cross(tar, tmp), 0), glm::vec4(tar, 0), glm::vec4(pos, 1));
}

glm::mat4 lookat(glm::vec3 eye, glm::vec3 center, glm::vec3 up = glm::vec3(0.0, 1.0, 0.0))
{
    glm::vec3 f = glm::normalize(center - eye);
    glm::vec3 s = glm::cross(normalize(up), f);
    glm::vec3 u = glm::cross(f, s);
    return glm::mat4(glm::vec4(s, 1), glm::vec4(u, 1), glm::vec4(f, 2)*0.5f, glm::vec4(eye, 0));
}

class camera
{
public:

    camera(glm::vec3 pos, glm::vec3 tar)
    {
        focus = (pos-tar).length();
        p = inverse(glm::lookAtLH(pos, tar, glm::vec3(0, 1, 0)));
        //p = lookat2(pos, tar);
        blur = 0.1f;
    }

    ray getRay(glm::vec2 uv)
    {
        glm::vec4 dir = p*glm::vec4(uv, 1.0f, 0.0f);
        return ray(
            glm::vec3(p*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
            glm::normalize(glm::vec3(dir))
        );
    }

    float focus;
    float blur;
    glm::mat4 p;
};