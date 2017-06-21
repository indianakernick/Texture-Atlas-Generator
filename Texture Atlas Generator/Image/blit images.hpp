//
//  blit images.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 23/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef image_blit_images_hpp
#define image_blit_images_hpp

#include <vector>
#include "image.hpp"
#include "../Utils/types.hpp"
#include "../Utils/range.hpp"

class FormatError : public std::runtime_error {
public:
  FormatError();
};

template <typename UnsignedInt>
using Converter = UnsignedInt(*)(const UnsignedInt);

void blit(Image &, const Image &, PosPx2 = {0, 0});
template <typename UnsignedInt>
void convert(Image &, ptrdiff_t, size_t, size_t, const uint8_t *, Converter<UnsignedInt>);

Image makeBlitDst(SizePx, Image::Format);
void blitImages(Image &, Range<const Image *>, Range<const RectPx *>);
Image makeAndBlit(Range<const Image *>, Range<const RectPx *>, SizePx);
Image makeAndBlit(const std::vector<Image> &, const std::vector<RectPx> &, SizePx);

#endif
