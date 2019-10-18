#ifndef INCLUDE_VEC3_H
#define INCLUDE_VEC3_H

#include "linear_algebra.h"

namespace tracer {

struct Vec3 {
  typedef const Vec3 &InParam;

  Vec3() noexcept {}

  constexpr Vec3(const float x, const float y, const float z) noexcept
      : x(x), y(y), z(z) {}

  float x, y, z;
};

inline Vec3 &operator+=(Vec3 &left, Vec3::InParam right) noexcept {
  left.x += right.x;
  left.y += right.y;
  left.z += right.z;
  return left;
}

inline constexpr Vec3 operator+(Vec3::InParam left,
                                Vec3::InParam right) noexcept {
  return Vec3{left.x + right.x, left.y + right.y, left.z + right.z};
}

inline constexpr Vec3 operator-(Vec3::InParam left,
                                Vec3::InParam right) noexcept {
  return Vec3{left.x - right.x, left.y - right.y, left.z - right.z};
}

inline constexpr Vec3 operator*(Vec3::InParam v, const float scale) noexcept {
  return Vec3{v.x * scale, v.y * scale, v.z * scale};
}

inline Vec3 &operator*=(Vec3 &v, const float scale) noexcept {
  v.x *= scale;
  v.y *= scale;
  v.z *= scale;
  return v;
}

inline constexpr float dot(Vec3::InParam left, Vec3::InParam right) noexcept {
  return left.x * right.x + left.y * right.y + left.z * right.z;
}

inline constexpr float length_squared(Vec3::InParam in) noexcept {
  return length_squared(in.x, in.y, in.z);
}

inline float length(Vec3::InParam in) noexcept {
  return length(in.x, in.y, in.z);
}

inline Vec3 normalised(Vec3::InParam in) noexcept {
  const float scale = 1.0f / length(in);
  return in * scale;
}

inline constexpr Vec3 lerp(Vec3::InParam from, Vec3::InParam to,
                           const float t) noexcept {
  return (from * (1.0f - t)) + (to * t);
}

} // namespace tracer

#endif // INCLUDE_VEC3_H

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
