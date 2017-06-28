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

void blit(Image &dst, const Image &src, const VecPx srcPos) {
  PROFILE(blit);
  
  if (dst.format != src.format) {
    throw FormatError();
  }

  const ptrdiff_t dstPitch = dst.pitch;
  const ptrdiff_t srcPitch = src.pitch;
  const size_t width = src.s.x * static_cast<CoordPx>(src.format);
  uint8_t *dstRow = dst.data.get() + (srcPos.y * dstPitch + srcPos.x * static_cast<CoordPx>(src.format));
  const uint8_t *srcRow = src.data.get();
  const uint8_t *const srcEnd = srcRow + srcPitch * src.s.y;
  
  while (srcRow != srcEnd) {
    std::memcpy(dstRow, srcRow, width);
    dstRow += dstPitch;
    srcRow += srcPitch;
  }
}

template <typename UnsignedInt>
void convert(Image &dst, const Image &src, const Converter<UnsignedInt> converter) {
  static_assert(std::is_unsigned<UnsignedInt>::value);

  PROFILE(Convert and copy);
  
  assert(static_cast<CoordPx>(dst.format) == sizeof(UnsignedInt));
  assert(dst.s == src.s);
  
  const ptrdiff_t dstStride = dst.pitch - dst.s.x * sizeof(UnsignedInt);
  const ptrdiff_t srcStride = src.pitch - src.s.x * sizeof(UnsignedInt);
  uint8_t *dstRow = dst.data.get();
  const uint8_t *srcRow = src.data.get();
  const ptrdiff_t srcWidth = src.s.x * sizeof(UnsignedInt);
  const uint8_t *const srcEnd = srcRow + src.pitch * src.s.y;
  
  while (srcRow != srcEnd) {
    const uint8_t *const end = srcRow + srcWidth;
    while (srcRow != end) {
      *reinterpret_cast<UnsignedInt *>(dstRow) =
        converter(*reinterpret_cast<const UnsignedInt *>(srcRow));
      dstRow += sizeof(UnsignedInt);
      srcRow += sizeof(UnsignedInt);
    }
    dstRow += dstStride;
    srcRow += srcStride;
  }
}

template void convert<uint32_t>(Image &, const Image &, Converter<uint32_t>);

template <typename UnsignedInt>
void convert(Image &dst, const Converter<UnsignedInt> converter) {
  static_assert(std::is_unsigned<UnsignedInt>::value);
  
  PROFILE(Convert in place);
  
  assert(static_cast<CoordPx>(dst.format) == sizeof(UnsignedInt));
  
  const ptrdiff_t dstStride = dst.pitch - dst.s.x * sizeof(UnsignedInt);
  
  if (dstStride == 0) {
    UnsignedInt *dstPx = reinterpret_cast<UnsignedInt *>(dst.data.get());
    UnsignedInt *const dstEnd = reinterpret_cast<UnsignedInt *>(dst.data.get() + (dst.pitch * dst.s.y));
    while (dstPx != dstEnd) {
      *dstPx = converter(*dstPx);
      dstPx++;
    }
  } else {
    uint8_t *dstPx = dst.data.get();
    uint8_t *const dstEnd = dst.data.get() + (dst.pitch * dst.s.y);
    const ptrdiff_t dstWidth = dst.s.x * sizeof(UnsignedInt);
    while (dstPx != dstEnd) {
      const uint8_t *const end = dstPx + dstWidth;
      while (dstPx != end) {
        *reinterpret_cast<UnsignedInt *>(dstPx) =
          converter(*reinterpret_cast<const UnsignedInt *>(dstPx));
        dstPx += sizeof(UnsignedInt);
      }
      dstPx += dstStride;
    }
  }
}

template void convert<uint32_t>(Image &, Converter<uint32_t>);

Image makeBlitDst(const CoordPx length, const Image::Format format) {
  return {length, length, format, 0};
}

void blitImages(Image &image, const Range<const Image *> images, const Range<const RectPx *> rects) {
  assert(images.size() == rects.size());
  
  PROFILE(blitImages);
  
  std::cout << "Copying smaller images onto larger image\n";
  
  if (images.size() == 0) {
    return;
  }
  
  const Image::Format imageFormat = image.format;
  for (const Image *i = images.cbegin(); i != images.cend(); i++) {
    if (i->format != imageFormat) {
      throw FormatError();
    }
  }

  for (size_t i = 0; i != images.size(); i++) {
    blit(image, images[i], {rects[i].x, rects[i].y});
  }
}

Image makeAndBlit(Range<const Image *> images, Range<const RectPx *> rects, const CoordPx length) {
  Image image = makeBlitDst(length, images.size() ? images.front().format : Image::Format::GREY);
  blitImages(image, images, rects);
  return image;
}

Image makeAndBlit(const std::vector<Image> &images, const std::vector<RectPx> &rects, const CoordPx length) {
  return makeAndBlit(makeRange(images), makeRange(rects), length);
}
