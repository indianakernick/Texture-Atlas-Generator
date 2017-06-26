//
//  math.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 24/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef utils_math_hpp
#define utils_math_hpp

#include <cassert>
#include "types.hpp"

inline unsigned int ceilToPowerOf2(const unsigned int num) {
  return (1 << (sizeof(int) * 8 - __builtin_clz(num - 1))) - (num == 1);
}

inline unsigned long ceilToPowerOf2(const unsigned long num) {
  return (1 << (sizeof(long) * 8 - __builtin_clzl(num - 1))) - (num == 1);
}

inline unsigned long long ceilToPowerOf2(const unsigned long long num) {
  return (1 << (sizeof(long long) * 8 - __builtin_clzll(num - 1))) - (num == 1);
}

inline SizePx ceilToPowerOf2(const SizePx num) {
  assert(num >= 0);
  return ceilToPowerOf2(static_cast<std::make_unsigned_t<SizePx>>(num));
}

#endif
