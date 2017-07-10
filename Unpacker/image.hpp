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
#ifdef BUILDING_PACKER
#include "../Utils/types.hpp"
#else
#pragma GCC visibility push(default)
namespace Unpack {
#include "types.hpp"
#endif

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
  Image(Image &&) = default;
  
  Image &operator=(Image &&) = default;

  uint8_t *data();
  uint8_t *data(VecPx);
  uint8_t *dataEnd();
  
  const uint8_t *data() const;
  const uint8_t *data(VecPx) const;
  const uint8_t *dataEnd() const;

  ptrdiff_t pitch() const;
  ptrdiff_t stride() const;
  
  CoordPx width() const;
  ptrdiff_t widthBytes() const;
  CoordPx height() const;
  
  Format format() const;

private:
  Data mData;
  ptrdiff_t mPitch;   //number of bytes between (x, y) and (x, y+1)
  CoordPx mWidth;     //width in pixels
  CoordPx mHeight;    //height in pixels
  Format mFormat;     //bytes per pixel
};

#if !defined(BUILDING_PACKER)
} //namespace Unpack
#pragma GCC visibility pop
#endif

#endif
