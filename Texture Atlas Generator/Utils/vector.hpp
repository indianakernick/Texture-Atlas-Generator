//
//  vector.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 24/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef utils_vector_hpp
#define utils_vector_hpp

template <typename T>
class tvec2 {
public:
  tvec2() = default;
  tvec2(const T x, const T y)
    : x(x), y(y) {}
  template <typename X, typename Y>
  tvec2(const X x, const Y y)
    : x(static_cast<T>(x)), y(static_cast<T>(y)) {}

  T x = 0;
  T y = 0;
};

#endif
