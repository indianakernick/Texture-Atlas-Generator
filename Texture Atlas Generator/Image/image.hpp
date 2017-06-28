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
#include "../Utils/types.hpp"

void defaultDelete(void *);
void noDelete(void *);

class Image {
public:
  using Deleter = void (*)(void *);
  using Data = std::unique_ptr<uint8_t, Deleter>;
  
  enum class Format : CoordPx {
    GREY       = 1, //STBI_grey
    GREY_ALPHA = 2, //STBI_grey_alpha
    RGB        = 3, //STBI_rgb
    RGB_ALPHA  = 4  //STBI_rgb_alpha
  };
  
  Image() = delete;
  Image(CoordPx, CoordPx, Format);
  Image(CoordPx, CoordPx, Format, uint8_t);
  Image(CoordPx, CoordPx, Format, ptrdiff_t, uint8_t *, Deleter);
  
  Data data;
  ptrdiff_t pitch;   //number of bytes between (x, y) and (x, y+1)
  CoordPx width;     //width in pixels
  CoordPx height;    //height in pixels
  Format format;     //bytes per pixel
};

#endif
