//
//  blit images.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 23/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "blit images.hpp"

#include <cassert>
#include <iostream>
#include "../Utils/profiler.hpp"
#include "../Utils/make range from vector.hpp"

FormatError::FormatError()
  : std::runtime_error("Cannot blit images of different formats") {}

// Arial ASCII 1000pt
//   this 26.87ms
//   prev 75.95ms
//   this is 182% faster than prev
void blit(Image &dst, const Image &src, const PosPx2 srcPos) {
  assert(dst.format == src.format);
  
  PROFILE(blit);

  const ptrdiff_t dstPitch = dst.s.x * dst.format;
  const ptrdiff_t srcPitch = src.s.x * src.format;
  const size_t width = srcPitch;
  uint8_t *dstRow = dst.data.get() + srcPos.y * dstPitch + srcPos.x * src.format;
  const uint8_t *srcRow = src.data.get();
  
  for (SizePx y = 0; y != src.s.y; y++) {
    std::memcpy(dstRow, srcRow, width);
    dstRow += dstPitch;
    srcRow += srcPitch;
  }
}

Image makeBlitDst(SizePx length, Image::Format format) {
  return {length, length, format, 0};
}

void blitImages(Image &image, Range<const Image *> images, Range<const RectPx *> rects) {
  assert(images.size() == rects.size());
  
  PROFILE(blitImages);
  
  std::cout << "Copying smaller images onto larger image\n";
  
  if (images.size() == 0) {
    return;
  }
  
  const Image::Format frontFormat = images.front().format;
  if (image.format != frontFormat) {
    throw FormatError();
  }
  
  for (const Image *i = images.cbegin() + 1; i != images.cend(); i++) {
    if (i->format != frontFormat) {
      throw FormatError();
    }
  }

  for (size_t i = 0; i != images.size(); i++) {
    blit(image, images[i], rects[i].p);
  }
}

Image makeAndBlit(Range<const Image *> images, Range<const RectPx *> rects, SizePx length) {
  Image image = makeBlitDst(length, images.size() ? images.front().format : Image::Format::GREY);
  blitImages(image, images, rects);
  return image;
}

Image makeAndBlit(const std::vector<Image> &images, const std::vector<RectPx> &rects, SizePx length) {
  return makeAndBlit(makeRange(images), makeRange(rects), length);
}
