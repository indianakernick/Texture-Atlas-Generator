//
//  write image.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef image_write_image_hpp
#define image_write_image_hpp

#include <string>
#include "image.hpp"

class ImageWriteError final : public std::runtime_error {
public:
  ImageWriteError();
};

void writeImage(const std::string &, const Image &);

#endif
