//
//  types.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 6/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef utils_types_hpp
#define utils_types_hpp

#pragma pack(1)
template <typename T>
struct Vec2 {
  using scalar = T;

  Vec2() = default;
  Vec2(const T x, const T y)
    : x(x), y(y) {}
  template <typename X, typename Y>
  Vec2(const X x, const Y y)
    : x(static_cast<T>(x)),
      y(static_cast<T>(y)) {}

  bool operator==(const Vec2 other) const {
    return x == other.x && y == other.y;
  }
  bool operator!=(const Vec2 other) const {
    return x != other.x || y != other.y;
  }

  T x = 0;
  T y = 0;
};

template <typename T>
struct Rect {
  using scalar = T;

  Rect() = default;
  Rect(const Vec2<T> p, const Vec2<T> s)
    : x(p.x), y(p.y), w(s.x), h(s.y) {}
  template <typename X, typename Y, typename W, typename H>
  Rect(const X x, const Y y, const W w, const H h)
    : x(static_cast<T>(x)),
      y(static_cast<T>(y)),
      w(static_cast<T>(w)),
      h(static_cast<T>(h)) {}
  
  bool operator==(const Rect other) const {
    return x == other.x && y == other.y && w == other.w && h == other.h;
  }
  bool operator!=(const Rect other) const {
    return x != other.x || y != other.y || w != other.w || h != other.h;
  }

  T x = 0;
  T y = 0;
  T w = 0;
  T h = 0;
};
#pragma pack()

using CoordPx = int32_t;
using VecPx = Vec2<CoordPx>;
using RectPx = Rect<CoordPx>;

#endif
