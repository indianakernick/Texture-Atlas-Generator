//
//  resize image.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "resize image.hpp"

#include <cassert>
#include "blit images.hpp"
#include "../Utils/profiler.hpp"
#include "../Libraries/stb_image_resize.h"

std::unique_ptr<uint8_t[]> temp = std::make_unique<uint8_t[]>(RESIZE_TEMP_SIZE);

Image resizePremulSRGB(const Image &image, const SizePx2 newSize) {
  PROFILE(Resize image);
  
  assert(image.format == Image::Format::RGB_ALPHA);
  
  Image newImage(newSize.x, newSize.y, Image::Format::RGB_ALPHA);
  
  stbir_resize_uint8_generic(
    image.data.get(), image.s.x, image.s.y, 0,
    newImage.data.get(), newImage.s.x, newImage.s.y, 0,
    4,//num_channels
    3,//alpha_channel
    STBIR_FLAG_ALPHA_PREMULTIPLIED,//flags
    STBIR_EDGE_ZERO,
    STBIR_FILTER_CATMULLROM,
    STBIR_COLORSPACE_SRGB,
    temp.get()
  );
  
  return newImage;
}
