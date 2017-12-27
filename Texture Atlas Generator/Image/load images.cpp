//
//  load images.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "load images.hpp"

#include <Surface/load.hpp>
#include "../Utils/logger.hpp"

NoLoadedImagesError::NoLoadedImagesError()
  : std::runtime_error("No images successfully loaded") {}

std::vector<Surface> loadImages(const std::vector<std::string> &files) {
  std::vector<Surface> images;
  
  for (auto f = files.cbegin(); f != files.cend(); ++f) {
    try {
      Logger::get() << "Loading image \"" << *f << "\"\n";
      images.emplace_back(loadSurfaceRGBA(*f));
    } catch (SurfaceLoadError &e) {
      std::cerr << e.what() << '\n';
    }
  }
  
  if (images.size() == 0) {
    throw NoLoadedImagesError();
  }
  
  return images;
}
