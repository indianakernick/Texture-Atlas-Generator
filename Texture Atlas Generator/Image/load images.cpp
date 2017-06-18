//
//  load images.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "load images.hpp"

#include <iostream>
#include "../Libraries/stb_image.h"
#include "../Utils/profiler.hpp"

ImageLoadError::ImageLoadError(const std::string &file, const std::string &reason)
  : std::runtime_error("Failed to load image: \"" + file + "\": " + reason) {}

NoLoadedImagesError::NoLoadedImagesError()
  : std::runtime_error("No images successfully loaded") {}

Image loadImage(const std::string &file) {
  PROFILE(loadImage);

  std::cout << "Loading image \"" << file << "\"\n";

  const Image::Format format = Image::Format::RGB_ALPHA;

  int width, height;
  uint8_t *data = stbi_load(file.c_str(), &width, &height, nullptr, format);
  if (data == nullptr) {
    throw ImageLoadError(file, stbi_failure_reason());
  }
  
  return {
    static_cast<SizePx>(width),
    static_cast<SizePx>(height),
    format,
    data,
    stbi_image_free
  };
}

std::vector<Image> loadImages(const std::vector<std::string> &files) {
  PROFILE(loadImages);

  std::vector<Image> images;
  
  for (auto f = files.begin(); f != files.end(); f++) {
    try {
      images.emplace_back(loadImage(*f));
    } catch (ImageLoadError &e) {
      std::cerr << e.what() << '\n';
    }
  }
  
  if (images.size() == 0) {
    throw NoLoadedImagesError();
  }
  
  return images;
}
