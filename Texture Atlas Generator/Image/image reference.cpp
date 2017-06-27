//
//  image reference.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 27/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "image reference.hpp"

BoundsError::BoundsError()
  : std::range_error("Rectangle out of range of image") {}

Image imageRef(const Image &src, const RectPx rect) {
  if (
    rect.p.x < 0 ||
    rect.p.y < 0 ||
    rect.s.x < 1 ||
    rect.s.y < 1 ||
    rect.p.x + rect.s.x > src.s.x ||
    rect.p.y + rect.s.y > src.s.y
  ) {
    throw BoundsError();
  }
  
  return {
    rect.s.x,
    rect.s.y,
    src.format,
    src.pitch,
    src.data.get() + (rect.y * src.pitch + rect.x * static_cast<SizePx>(src.format)),
    noDelete
  };
}
