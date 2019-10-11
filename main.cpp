#include <fstream>
#include <float.h>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include "ray.hpp"
#include "sphere.hpp"
#include "scene.hpp"
#include "camera.hpp"


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
    }
    
    float t = (r.direction.y + 1.0f)*0.5f;
    return glm::vec3(1.0f, 1.0f, 1.0f)*(1.0f - t)+glm::vec3(0.5f, 0.7f, 1.0f)*t;
}

int main()
{
    int IMG_WIDTH = 400;
    int IMG_HEIGHT = 400;
    int SAMPLE_COUNT = 16;

    std::ofstream image("result.ppm");

    image << "P3\n" << IMG_WIDTH << " " << IMG_HEIGHT << "\n255\n";

    glm::vec3 lookfrom(0.0, 0.0, 1.0);
    glm::vec3 lookat(0.0, 0.0, -1.0);
    float dist_to_focus = (lookfrom - lookat).length();

    body *list[5];
    list[0] = new sphere(glm::vec3(0.0, 0.0, -1.0), 0.5, new lambertian(glm::vec3(0.8, 0.3, 0.3)));
    list[1] = new sphere(glm::vec3(0.0, -100.5, -1.0), 100.0, new lambertian(glm::vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(glm::vec3(1.0, 0.0, -1.0), 0.5, new metal(glm::vec3(0.8, 0.6, 0.2), 0.3));
    list[3] = new sphere(glm::vec3(-1.0, 0.0, -1.0), 0.5, new dielectric(1.5));
    list[4] = new sphere(glm::vec3(-1.0, 0.0, -1.0), -0.45, new dielectric(1.5));
    body *world = new scene(list, 5);

    camera cam(lookfrom, lookat);

    for( int j = IMG_HEIGHT/2; j>= -IMG_HEIGHT/2; j-- )
    for( int i = -IMG_WIDTH/2; i < IMG_WIDTH/2; i++ )
    {
        glm::vec3 col(0,0,0);
        for( int k = 0; k<SAMPLE_COUNT; k++ )
        {
            glm::vec2 uv = ( glm::vec2(i, j) +
            glm::linearRand(glm::vec2(-0.5 -0.5), glm::vec2(0.5, 0.5)))/glm::vec2(IMG_WIDTH, IMG_HEIGHT);
            //(glm::vec2(i, j)-glm::vec2(IMG_WIDTH, IMG_HEIGHT)/2.0f
            //+ glm::linearRand(glm::vec2(-0.5 -0.5), glm::vec2(0.5, 0.5)))/float(glm::min(IMG_WIDTH, IMG_HEIGHT));

            ray r = cam.getRay(uv);
            col = col + color(r, world);
        }

        col = col/float(SAMPLE_COUNT);
        image << int(sqrt(col[0])*255.99f) << " " << int(sqrt(col[1])*255.99f) << " " << int(sqrt(col[2])*255.99f) << "\n";
    }


    return 0;
}
