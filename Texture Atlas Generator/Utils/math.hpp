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
#include <type_traits>

inline unsigned int ceilToPowerOf2(const unsigned int num) {
  return (1 << (sizeof(int) * 8 - __builtin_clz(num - 1))) - (num == 1);
}

inline unsigned long ceilToPowerOf2(const unsigned long num) {
  return (1 << (sizeof(long) * 8 - __builtin_clzl(num - 1))) - (num == 1);
}

inline unsigned long long ceilToPowerOf2(const unsigned long long num) {
  return (1 << (sizeof(long long) * 8 - __builtin_clzll(num - 1))) - (num == 1);
}

inline bool isPowerOf2(const unsigned int num) {
  assert(num != 0);
  return (__builtin_clz(num) + __builtin_ctz(num)) == sizeof(int) * 8 - 1;
}

inline bool isPowerOf2(const unsigned long num) {
  assert(num != 0);
  return (__builtin_clzl(num) + __builtin_ctzl(num)) == sizeof(long) * 8 - 1;
}

inline bool isPowerOf2(const unsigned long long num) {
  assert(num != 0);
  return (__builtin_clzll(num) + __builtin_ctzll(num)) == sizeof(long long) * 8 - 1;
}

template <typename NUM, typename DEN>
std::common_type_t<NUM, DEN> divRound(const NUM num, const DEN den) {
  return ((num < 0) ^ (den < 0)) ? (num - den / 2) / den
                                 : (num + den / 2) / den;
}

#endif
