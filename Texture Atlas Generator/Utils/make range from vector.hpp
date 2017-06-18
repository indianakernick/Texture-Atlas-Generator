//
//  make range from vector.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 13/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef utils_make_range_from_vector_hpp
#define utils_make_range_from_vector_hpp

#include <vector>
#include "range.hpp"

template <typename T>
Range<const T *> makeRange(const std::vector<T> &container) {
  return {container.data(), container.data() + container.size()};
}

template <typename T>
Range<T *> makeRange(std::vector<T> &container) {
  return {container.data(), container.data() + container.size()};
}

#endif
