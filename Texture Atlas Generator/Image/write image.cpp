//
//  write image.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "write image.hpp"

#include <iostream>
#include "../Utils/profiler.hpp"
#include "../Libraries/stb_image_write.h"

ImageWriteError::ImageWriteError()
  : std::runtime_error("Failed to write image to file") {}

void writeImage(const std::string &file, const Image &image) {
  PROFILE(writeImage);
  
  std::cout << "Writing image to file \"" << file << "\"\n";
  
  const int success = stbi_write_png(
    file.c_str(),
    image.s.x,
    image.s.y,
    image.format,
    image.data.get(),
    0
  );
  
  if (success == 0) {
    throw ImageWriteError();
  }
}
