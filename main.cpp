#include "vec3.h"

#include <stdio.h>

namespace tracer {

constexpr Vec3 reflected(Vec3::InParam vector, Vec3::InParam normal) noexcept {
  // To reflect the supplied vector along the axis of the normal, add as much
  // of the normal to the vector as would make it perpendicular to the
  // normal, then add the same amount again.
  //
  // This will reverse the vector's magnitude with respect to the normal,
  // whilst keeping the magnitude of the vector the same.
  //
  const float magnitude_of_normal_shared_with_vector = dot(vector, normal);
  const float normal_scale = -2.0f * magnitude_of_normal_shared_with_vector;
  return vector + normal * normal_scale;
}

constexpr Vec3 sample_sky(Vec3::InParam direction) noexcept {
  constexpr Vec3 up{0.0f, 1.0f, 0.0f};

  constexpr float red = 32.0f;
  constexpr float green = 128.0f;
  const float blue = 180.0f + 64.0f * dot(direction, up);

  return Vec3{red, green, blue};
}

Vec3 sample_ground(Vec3::InParam position, Vec3::InParam direction,
                   Vec3::InParam sphere_point_on_ground,
                   const float sphere_magnitude_squared) noexcept {
  constexpr float inverse_square_scale = 0.1f;
  const int x = fabs(position.x * inverse_square_scale);
  const int y = fabs(position.z * inverse_square_scale);

  constexpr Vec3 world_down{0.0f, -1.0f, 0.0f};
  const float dot_down{dot(direction, world_down)};

  Vec3 colour;

  if ((x % 2) ^ (y % 2)) {
    constexpr Vec3 green{32.0f, 128.0f, 32.0f};
    constexpr Vec3 light_green{green * 2.0f};
    colour = lerp(green, light_green, dot_down);
  } else {
    constexpr Vec3 green{64.0f, 128.0f, 64.0f};
    constexpr Vec3 light_green{green * 2.0f};
    colour = lerp(green, light_green, dot_down);
  }

  const Vec3 to_sphere_on_ground = sphere_point_on_ground - position;
  if (length_squared(to_sphere_on_ground) < sphere_magnitude_squared) {
    colour *= 0.5f;
  }

  return colour;
}

Vec3 sample_sphere(Vec3::InParam position, Vec3::InParam normal,
                   Vec3::InParam direction) noexcept {
  constexpr Vec3 world_up{0.0f, 1.0f, 0.0f};
  const float dot_up{dot(normal, world_up)};

  constexpr Vec3 half{0.5f, 0.5f, 0.5f};
  const Vec3 color{(normal * 0.5f + half) * 255.0f};
  const Vec3 half_color{color * 0.5f};
  return lerp(half_color, color, dot_up);
}

Vec3 sample(Vec3 position, Vec3 direction) noexcept {

  Vec3 colour{0.0f, 0.0f, 0.0f};
  float colour_blend = 1.0f;

  // check for collision against sphere
  constexpr Vec3 sphere_point{0.0f, 15.0f, 40.0f};
  constexpr Vec3 sphere_point_on_ground{0.0f, 0.0f, 40.0f};
  constexpr float sphere_magnitude = 11.0f;
  constexpr float sphere_magnitude_squared =
      sphere_magnitude * sphere_magnitude;
  const Vec3 to_sphere{sphere_point - position};
  const float closest_to_sphere{dot(direction, to_sphere)};
  const Vec3 point_nearest_sphere{position + direction * closest_to_sphere};
  const float length_to_sphere{length(sphere_point - point_nearest_sphere)};
  // TODO: use squared magnitude
  if (length_to_sphere < sphere_magnitude) {
    // calc intersection point
    const float adjacent_length = sqrt(sphere_magnitude * sphere_magnitude -
                                       length_to_sphere * length_to_sphere);
    const Vec3 intersection{point_nearest_sphere -
                            (direction * adjacent_length)};

    // sample the sphere
    const Vec3 normal{normalised(intersection - sphere_point)};
    position = intersection;
    direction = reflected(direction, normal);
    colour = lerp(colour, sample_sphere(intersection, normal, direction),
                  colour_blend);
    colour_blend = 0.5f;
  }

  // check for collision against ground
  constexpr Vec3 ground_inverse_normal(0.0f, -1.0f, 0.0f);
  if (dot(ground_inverse_normal, direction) > 0.0f) {
    // hit the ground, figure out where
    constexpr Vec3 ground_point(0.0f, 0.0f, 0.0f);
    const Vec3 to_ground{ground_point - position};
    const float distance_to_ground{dot(to_ground, ground_inverse_normal)};
    const float dot_to_ground{dot(direction, ground_inverse_normal)};
    const float distance{distance_to_ground / dot_to_ground};
    const Vec3 ground_intersection{position + direction * distance};
    colour =
        lerp(colour,
             sample_ground(ground_intersection, direction,
                           sphere_point_on_ground, sphere_magnitude_squared),
             colour_blend);
  } else {
    colour = lerp(colour,
                  // TODO: remove position parameter
                  sample_sky(direction), colour_blend);
  }

  return colour;
}

void draw_scene(const int image_width, const int image_height) noexcept {
  constexpr Vec3 camera_forward{0.0f, 0.0f, 1.0f};
  constexpr Vec3 camera_pos{0.0f, 10.0f, 0.0f};

  const int half_image_width = image_width / 2;
  const int half_image_height = image_height / 2;

  for (int y = image_height - 1; y >= 0; --y) {
    for (int x = 0; x < image_width; ++x) {
      const Vec3 pixel_offset =
          Vec3{float(x - half_image_width), float(y - half_image_height), 0.0f};

      const Vec3 pixel_direction =
          normalised(camera_forward * image_height + pixel_offset);

      const Vec3 colour = sample(camera_pos, pixel_direction);

      printf("%c%c%c", int(colour.x), int(colour.y), int(colour.z));
    }
  }
}

} // namespace tracer

int main() {
  printf("P6 512 512 255 ");
  tracer::draw_scene(512, 512);
}

// Copyright 2014-2019 Angelos Evripiotis
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
