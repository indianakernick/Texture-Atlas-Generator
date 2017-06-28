//
//  image.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef unpacker_image_hpp
#define unpacker_image_hpp

#include <memory>

#pragma pack(1)
template <typename T>
class tvec2 {
public:
  using scalar = T;

  tvec2() = default;
  tvec2(const T x, const T y)
    : x(x), y(y) {}
  template <typename X, typename Y>
  tvec2(const X x, const Y y)
    : x(static_cast<T>(x)), y(static_cast<T>(y)) {}

  bool operator==(const tvec2 other) const {
    return x == other.x && y == other.y;
  }
  bool operator!=(const tvec2 other) const {
    return x != other.x || y != other.y;
  }

  T x = 0;
  T y = 0;
};
#pragma pack()

using PosPx = int32_t;
using SizePx = int32_t;
using PosPx2 = tvec2<PosPx>;
using SizePx2 = tvec2<SizePx>;

#pragma pack(1)
struct RectPx {
  PosPx2 p;
  SizePx2 s;
};
#pragma pack()

void defaultDelete(void *);
void noDelete(void *);

class Image {
public:
  using Deleter = void (*)(void *);
  using Data = std::unique_ptr<uint8_t, Deleter>;
  
  enum class Format : SizePx {
    GREY       = 1, //STBI_grey
    GREY_ALPHA = 2, //STBI_grey_alpha
    RGB        = 3, //STBI_rgb
    RGB_ALPHA  = 4  //STBI_rgb_alpha
  };
  
  Image() = delete;
  Image(SizePx, SizePx, Format);
  Image(SizePx, SizePx, Format, uint8_t);
  Image(SizePx, SizePx, Format, ptrdiff_t, uint8_t *, Deleter);
  
  Data data;
  ptrdiff_t pitch;                  //number of bytes between (x, y) and (x, y+1)
  SizePx2 s;                        //area of pixels
  Format format = Format::RGB_ALPHA;//bytes per pixel
};

#endif
