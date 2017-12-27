//
//  load images.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef image_load_images_hpp
#define image_load_images_hpp

#include <vector>
#include <string>
#include <string_view>
#include <Surface/surface.hpp>

class NoLoadedImagesError final : public std::runtime_error {
public:
  NoLoadedImagesError();
};

std::vector<Surface> loadImages(const std::vector<std::string> &);

#endif
