#include <math.h>
#include <stdio.h>

struct Vec3
{
    Vec3() {}

    Vec3(const float x, const float y, const float z)
        : x(x)
        , y(y)
        , z(z)
    {
    }

    float x, y, z;
};

// Vec3&& normalize(Vec3&& in) {
//     return Vec3
// }

float distance2d(float x, float y) {
    return sqrt(x*x + y*y);
}

void colourful_square() {
    Vec3 colour(0.0f, 0.0f, 0.0f);
    float increment = 255.f / 512.f;

    for (int x=0; x<512; ++x) {
        colour.x += increment;
        colour.y = 0;
        for (int y=0; y<512; ++y) {
            colour.y += increment;
            colour.z = 64.0f + distance2d(x, y) / 8.0f;
            printf("%c%c%c", int(colour.x), int(colour.y), int(colour.z));
        }
    }
}

int main() {
    printf("P6 512 512 255 ");
    colourful_square();
}
