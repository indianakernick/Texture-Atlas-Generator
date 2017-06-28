//
//  load images.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef unpacker_load_images_hpp
#define unpacker_load_images_hpp

#include <string>
#include "image.hpp"

class ImageLoadError final : public std::runtime_error {
public:
  ImageLoadError(const std::string &, const std::string &);
};

Image loadImage(const std::string &);

#endif
