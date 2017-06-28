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
    image.width,
    image.height,
    static_cast<int>(image.format),
    image.data.get(),
    static_cast<int>(image.pitch - image.width * static_cast<CoordPx>(image.format))
  );
  
  if (success == 0) {
    throw ImageWriteError();
  }
}
