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

Vec3 operator-(Vec3::InParam left, Vec3::InParam right) {
    return Vec3{
        left.x - right.x,
        left.y - right.y,
        left.z - right.z
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

Vec3 lerp(Vec3::InParam from, Vec3::InParam to, const float t) {
    return (from * (1.0f - t) + (to * t));
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

Vec3 sample_sky(const Vec3::InParam position, const Vec3::InParam direction) {
    const Vec3 forward{0.0f, 0.0f, 1.0f};
    const Vec3 right{1.0f, 0.0f, 0.0f};
    const Vec3 up{0.0f, 1.0f, 0.0f};

    const float red = 32.0f;
    const float green = 128.0f;
    const float blue = 180.0f + 64.0f * dot(direction, up);

    return Vec3 {red, green, blue};
}

Vec3 sample_ground(
        const Vec3::InParam position,
        const Vec3::InParam direction)
{
    const float inverse_square_scale = 0.1f;
    const int x = fabs(position.x * inverse_square_scale);
    const int y = fabs(position.z * inverse_square_scale);

    const Vec3 world_up {0.0f, 1.0f, 0.0f};
    const float dot_up {dot(direction, world_up)};

    if ((x%2) ^ (y%2)) {
        const Vec3 green {32.0f, 128.0f, 32.0f};
        const Vec3 light_green {green * 2.0f};
        return lerp(green, light_green, dot_up);
    } else {
        const Vec3 green {64.0f, 128.0f, 64.0f};
        const Vec3 light_green {green * 2.0f};
        return lerp(green, light_green, dot_up);
    }
}

Vec3 sample(const Vec3::InParam position, const Vec3::InParam direction) {

    // check for collision against sphere
    const Vec3 sphere_point(0.0f, 7.5f, 50.0f);
    const float sphere_magnitude = 5.0f;
    const Vec3 to_sphere {sphere_point - position};
    const float closest_to_sphere {dot(direction, to_sphere)};
    const Vec3 point_nearest_sphere {position + direction * closest_to_sphere};
    const float length_to_sphere {length(sphere_point - point_nearest_sphere)};
    // TODO: use squared magnitude
    if (length_to_sphere < sphere_magnitude) {
        // calc intersection point
        const float adjacent_length = sqrt(
            sphere_magnitude * sphere_magnitude
            + length_to_sphere * length_to_sphere);
        const Vec3 intersection {
            point_nearest_sphere - (direction * adjacent_length)};

        // sample the sphere
        const Vec3 normal {normalised(intersection - sphere_point)};
        const Vec3 half {0.5f, 0.5f, 0.5f};
        const Vec3 color {(normal * 0.5f + half) * 255.0f};
        return color;
    }

    // check for collision against ground
    const Vec3 ground_point(0.0f, 0.0f, 0.0f);
    const Vec3 ground_inverse_normal(0.0f, -1.0f, 0.0f);
    const Vec3 to_ground {ground_point - position};
    if (dot(to_ground, direction) > 0.0f) {
        // hit the ground, figure out where
        const float distance_to_ground {dot(to_ground, ground_inverse_normal)};
        const float dot_to_ground {dot(direction, ground_inverse_normal)};
        const float distance {distance_to_ground / dot_to_ground};
        const Vec3 ground_intersection {position + direction * distance};
        return sample_ground(ground_intersection, direction);
    }

    return sample_sky(position, direction);
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
