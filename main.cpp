#include <math.h>
#include <stdio.h>

struct Vec3
{
    typedef const Vec3& InParam;

    Vec3() {}

    Vec3(const float x, const float y, const float z)
        : x(x)
        , y(y)
        , z(z)
    {
    }

    float x, y, z;
};

Vec3 operator+(Vec3::InParam left, Vec3::InParam right) {
    return Vec3{
        left.x + right.x,
        left.y + right.y,
        left.z + right.z
    };
}

Vec3 operator*(Vec3::InParam v, const float scale) {
    return Vec3 {
        v.x * scale,
        v.y * scale,
        v.z * scale
    };
}

float dot(Vec3::InParam left, Vec3::InParam right) {
    return left.x*right.x + left.y*right.y + left.z*right.z;
}

// Vec3&& normalize(Vec3&& in) {
//     return Vec3
// }

float distance2d(const float x, const float y) {
    return sqrt(x*x + y*y);
}

float distance3d(const float x, const float y, const float z) {
    return sqrt(x*x + y*y + z*z);
}

float length(Vec3::InParam in) {
    return distance3d(in.x, in.y, in.z);
}

Vec3 normalised(Vec3::InParam in) {
    const float scale = 1.0f / length(in);
    return in * scale;
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

Vec3 sample(const Vec3::InParam position, const Vec3::InParam direction) {
    const Vec3 forward{0.0f, 0.0f, 1.0f};
    const Vec3 right{1.0f, 0.0f, 0.0f};
    const Vec3 up{0.0f, 1.0f, 0.0f};

    const float red = 64.0f;
    const float green = 128.0f;
    const float blue = 127.0f + 127.0f * dot(direction, up);

    return Vec3 {red, green, blue};
}

void draw_scene(const int image_width, const int image_height) {
    const Vec3 camera_forward{0.0f, 0.0f, 1.0f};
    const Vec3 camera_right{1.0f, 0.0f, 0.0f};
    const Vec3 camera_up{0.0f, 1.0f, 0.0f};
    const Vec3 camera_pos{0.0f, 10.0f, 0.0f};

    const int half_image_width = image_width / 2;
    const int half_image_height = image_height / 2;

    for (int y=image_height-1; y>=0; --y) {
        for (int x=0; x<image_width; ++x) {
            const Vec3 pixel_offset = Vec3 {
                float(x-half_image_width),
                float(y-half_image_height),
                0.0f};

            const Vec3 pixel_direction = normalised(
                camera_forward*image_height + pixel_offset);

            const Vec3 colour = sample(camera_pos, pixel_direction);

            printf("%c%c%c", int(colour.x), int(colour.y), int(colour.z));
        }
    }
}

int main() {
    printf("P6 512 512 255 ");
    draw_scene(512, 512);
}
