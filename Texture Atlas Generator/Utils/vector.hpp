//
//  vector.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 24/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef utils_vector_hpp
#define utils_vector_hpp

#pragma pack(1)
template <typename T>
class tvec2 {
public:
  using scalar = T;

  tvec2() = default;
  tvec2(const T x, const T y)
    : x(x), y(y) {}
  template <typename X, typename Y>
  tvec2(const X x, const Y y)
    : x(static_cast<T>(x)), y(static_cast<T>(y)) {}

  bool operator==(const tvec2 other) const {
    return x == other.x && y == other.y;
  }
  bool operator!=(const tvec2 other) const {
    return x != other.x || y != other.y;
  }

  T x = 0;
  T y = 0;
};
#pragma pack()

#endif
