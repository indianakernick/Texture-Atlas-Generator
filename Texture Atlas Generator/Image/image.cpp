//
//  image.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "image.hpp"

#include <cstdlib>

uint8_t *defaultNew(SizePx width, SizePx height, Image::Format format) {
  return reinterpret_cast<uint8_t *>(
    std::malloc(width * height * static_cast<SizePx>(format))
  );
}

void defaultDelete(void *ptr) {
  std::free(ptr);
}

//stb_image_write calls realloc on the pointer stored in the image
//so the memory has to be allocated with malloc

Image::Image(SizePx width, SizePx height, Format format)
  : data(defaultNew(width, height, format), defaultDelete),
    s(width, height),
    format(format) {}

Image::Image(SizePx width, SizePx height, Format format, uint8_t byte)
  : Image(width, height, format) {
  std::memset(data.get(), byte, width * height * format);
}

Image::Image(
  SizePx width,
  SizePx height,
  Format format,
  uint8_t *data,
  Deleter deleter
) : data(data, deleter),
    s(width, height),
    format(format) {}
