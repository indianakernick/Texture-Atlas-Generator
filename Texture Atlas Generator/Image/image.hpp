//
//  image.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef image_image_hpp
#define image_image_hpp

#include <memory>
#include <string>
#include "../Utils/types.hpp"

class Image {
public:
  using Deleter = void (*)(void *);
  using Data = std::unique_ptr<uint8_t, Deleter>;
  
  enum Format : SizePx {
    GREY       = 1,//STBI_grey
    GREY_ALPHA = 2,//STBI_grey_alpha
    RGB        = 3,//STBI_rgb
    RGB_ALPHA  = 4 //STBI_rgb_alpha
  };
  
  Image() = delete;
  Image(SizePx, SizePx, Format);
  Image(SizePx, SizePx, Format, uint8_t);
  Image(SizePx, SizePx, Format, uint8_t *, Deleter);
  
  Data data;
  SizePx2 s;
  Format format = Format::RGB_ALPHA;
};

#endif
