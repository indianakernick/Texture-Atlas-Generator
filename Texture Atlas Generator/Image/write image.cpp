//
//  write image.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "write image.hpp"

#include "../Utils/logger.hpp"
#include "../Utils/profiler.hpp"
#include "../Libraries/stb_image_write.h"

ImageWriteError::ImageWriteError()
  : std::runtime_error("Failed to write image to file") {}

void writeImage(const std::experimental::string_view file, const Image &image) {
  PROFILE(writeImage);
  
  Logger::get() << "Writing image to file \"" << file << "\"\n";
  
  const int success = stbi_write_png(
    file.data(),
    image.width(),
    image.height(),
    static_cast<int>(image.format()),
    image.data(),
    static_cast<int>(image.stride())
  );
  
  if (success == 0) {
    throw ImageWriteError();
  }
}
