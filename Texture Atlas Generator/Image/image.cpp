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

uint8_t *defaultNew(const SizePx width, const SizePx height, const Image::Format format) {
  assert(width > 0);
  assert(height > 0);
  return reinterpret_cast<uint8_t *>(
    std::malloc(width * height * static_cast<SizePx>(format))
  );
}

void defaultDelete(void *ptr) {
  std::free(ptr);
}

void noDelete(void *) {}

//stb_image_write calls realloc on the pointer stored in the image
//so the memory has to be allocated with malloc

Image::Image(const SizePx width, const SizePx height, const Format format)
  : data(defaultNew(width, height, format), defaultDelete),
    pitch(width * static_cast<SizePx>(format)),
    s(width, height),
    format(format) {
  assert(width > 0);
  assert(height > 0);
}

Image::Image(const SizePx width, const SizePx height, const Format format, const uint8_t byte)
  : Image(width, height, format) {
  std::memset(data.get(), byte, width * height * static_cast<SizePx>(format));
}

Image::Image(
  const SizePx width,
  const SizePx height,
  const Format format,
  const ptrdiff_t pitch,
  uint8_t *const data,
  const Deleter deleter
) : data(data, deleter),
    pitch(pitch),
    s(width, height),
    format(format) {
  assert(width > 0);
  assert(height > 0);
}
