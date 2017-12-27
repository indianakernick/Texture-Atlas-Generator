//
//  write image.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "write image.hpp"

#include <Surface/write.hpp>
#include "../Utils/logger.hpp"

void writeImage(const std::string_view file, const Surface &image) {
  Logger::get() << "Writing image to file \"" << file << "\"\n";
  
  writeSurface(file, image);
}
