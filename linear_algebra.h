#ifndef INCLUDE_LINEAR_ALGEBRA_H
#define INCLUDE_LINEAR_ALGEBRA_H

#include <math.h>

namespace tracer {

inline constexpr float length_squared(const float x, const float y,
                                      const float z) noexcept {
  return x * x + y * y + z * z;
}

inline float length(const float x, const float y) noexcept {
  // sqrt may raise std::domain_error if the input is negative but that's not
  // possible here as we're summing two squares
  return sqrt(x * x + y * y);
}

inline float length(const float x, const float y, const float z) noexcept {
  // sqrt may raise std::domain_error if the input is negative but that's not
  // possible here as we're summing three squares
  return sqrt(x * x + y * y + z * z);
}

} // namespace tracer

#endif // INCLUDE_LINEAR_ALGEBRA_H

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
