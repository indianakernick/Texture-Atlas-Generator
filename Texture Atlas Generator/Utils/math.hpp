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

inline unsigned int ceilToPowerOf2Impl(const unsigned int num) {
  return (1 << (sizeof(int) * 8 - __builtin_clz(num - 1))) - (num == 1);
}

inline unsigned long ceilToPowerOf2Impl(const unsigned long num) {
  return (1 << (sizeof(long) * 8 - __builtin_clzl(num - 1))) - (num == 1);
}

inline unsigned long long ceilToPowerOf2Impl(const unsigned long long num) {
  return (1 << (sizeof(long long) * 8 - __builtin_clzll(num - 1))) - (num == 1);
}

template <typename Num>
Num ceilToPowerOf2(const Num num) {
  static_assert(std::is_integral<Num>::value);

  using Dest = std::conditional_t<
    sizeof(Num) < sizeof(unsigned int),
    unsigned int,
    std::make_unsigned_t<Num>
  >;

  assert(num >= 0);
  
  return ceilToPowerOf2Impl(static_cast<Dest>(num));
}

#endif
