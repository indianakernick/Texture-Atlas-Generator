//
//  image.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "image.hpp"

#include <cstdlib>
#include <cassert>

uint8_t *defaultNew(const CoordPx width, const CoordPx height, const Image::Format format) {
  assert(width > 0);
  assert(height > 0);
  return reinterpret_cast<uint8_t *>(
    std::malloc(width * height * static_cast<CoordPx>(format))
  );
}

void defaultDelete(void *ptr) {
  std::free(ptr);
}

void noDelete(void *) {}

//stb_image_write calls realloc on the pointer stored in the image
//so the memory has to be allocated with malloc

Image::Image(const CoordPx width, const CoordPx height, const Format format)
  : data(defaultNew(width, height, format), defaultDelete),
    pitch(width * static_cast<CoordPx>(format)),
    width(width),
    height(height),
    format(format) {
  assert(width > 0);
  assert(height > 0);
}

Image::Image(const CoordPx width, const CoordPx height, const Format format, const uint8_t byte)
  : Image(width, height, format) {
  std::memset(data.get(), byte, width * height * static_cast<CoordPx>(format));
}

Image::Image(
  const CoordPx width,
  const CoordPx height,
  const Format format,
  const ptrdiff_t pitch,
  uint8_t *const data,
  const Deleter deleter
) : data(data, deleter),
    pitch(pitch),
    width(width),
    height(height),
    format(format) {
  assert(width > 0);
  assert(height > 0);
}
