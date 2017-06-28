//
//  load images.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "load images.hpp"

#include "stb_image.h"

ImageLoadError::ImageLoadError(const std::string &file, const std::string &reason)
  : std::runtime_error("Failed to load image: \"" + file + "\": " + reason) {}

Image loadImage(const std::string &file) {
  const Image::Format format = Image::Format::RGB_ALPHA;

  int width, height;
  uint8_t *data = stbi_load(file.c_str(), &width, &height, nullptr, static_cast<int>(format));
  if (data == nullptr) {
    throw ImageLoadError(file, stbi_failure_reason());
  }
  
  return {
    static_cast<SizePx>(width),
    static_cast<SizePx>(height),
    format,
    static_cast<ptrdiff_t>(width * static_cast<SizePx>(format)),
    data,
    stbi_image_free
  };
}
