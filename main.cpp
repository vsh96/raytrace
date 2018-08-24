#include <fstream>

int main()
{
    int IMG_WIDTH = 400;
    int IMG_HEIGHT = 200;

    std::ofstream image("result.ppm");

    image << "P3\n" << IMG_WIDTH << " " << IMG_HEIGHT << "\n255\n";

    for( int j = IMG_HEIGHT-1; j>=0; j-- )
    for( int i = 0; i<IMG_WIDTH; i++ )
    {
        float r = float( i )/float(IMG_WIDTH);
        float g = float( j )/float(IMG_HEIGHT);
        float b = 0.2f;
        image << int(r*255.99f) << " " << int(g*255.99f) << " " << int(b*255.99f) << "\n";
    }


    return 0;
}
