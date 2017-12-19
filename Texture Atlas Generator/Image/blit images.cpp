//
//  blit images.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 23/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "blit images.hpp"

#include <cassert>
#include "../Utils/logger.hpp"
#include "../Utils/profiler.hpp"

FormatError::FormatError()
  : std::runtime_error("Cannot blit images of different formats") {}

void blit(Image &dst, const Image &src, const VecPx srcPos) {
  PROFILE(blit);
  
  if (dst.format() != src.format()) {
    throw FormatError();
  }

  const ptrdiff_t dstPitch = dst.pitch();
  const ptrdiff_t srcPitch = src.pitch();
  const size_t width = src.widthBytes();
  uint8_t *dstRow = dst.data(srcPos);
  const uint8_t *srcRow = src.data();
  const uint8_t *const srcEnd = src.dataEnd();
  
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
  
  assert(static_cast<CoordPx>(dst.format()) == sizeof(UnsignedInt));
  assert(dst.width() == src.width());
  assert(dst.height() == src.height());
  
  const ptrdiff_t dstPadding = dst.padding();
  const ptrdiff_t srcPadding = src.padding();
  uint8_t *dstRow = dst.data();
  const uint8_t *srcRow = src.data();
  const ptrdiff_t srcWidth = src.widthBytes();
  const uint8_t *const srcEnd = src.dataEnd();
  
  while (srcRow != srcEnd) {
    const uint8_t *const end = srcRow + srcWidth;
    while (srcRow != end) {
      *reinterpret_cast<UnsignedInt *>(dstRow) =
        converter(*reinterpret_cast<const UnsignedInt *>(srcRow));
      dstRow += sizeof(UnsignedInt);
      srcRow += sizeof(UnsignedInt);
    }
    dstRow += dstPadding;
    srcRow += srcPadding;
  }
}

//not needed at the moment
//template void convert<uint32_t>(Image &, const Image &, Converter<uint32_t>);

template <typename UnsignedInt>
void convert(Image &dst, const Converter<UnsignedInt> converter) {
  static_assert(std::is_unsigned<UnsignedInt>::value);
  
  PROFILE(Convert in place);
  
  assert(static_cast<CoordPx>(dst.format()) == sizeof(UnsignedInt));
  
  const ptrdiff_t dstPadding = dst.padding();
  
  if (dstPadding == 0) {
    UnsignedInt *dstPx = reinterpret_cast<UnsignedInt *>(dst.data());
    UnsignedInt *const dstEnd = reinterpret_cast<UnsignedInt *>(dst.dataEnd());
    while (dstPx != dstEnd) {
      *dstPx = converter(*dstPx);
      dstPx++;
    }
  } else {
    uint8_t *dstPx = dst.data();
    uint8_t *const dstEnd = dst.dataEnd();
    const ptrdiff_t dstWidth = dst.widthBytes();
    while (dstPx != dstEnd) {
      const uint8_t *const end = dstPx + dstWidth;
      while (dstPx != end) {
        *reinterpret_cast<UnsignedInt *>(dstPx) =
          converter(*reinterpret_cast<const UnsignedInt *>(dstPx));
        dstPx += sizeof(UnsignedInt);
      }
      dstPx += dstPadding;
    }
  }
}

//not needed at the moment
//template void convert<uint32_t>(Image &, Converter<uint32_t>);

Image makeBlitDst(const CoordPx length, const Image::Format format) {
  return {length, length, format, 0};
}

void blitImages(Image &image, const std::vector<Image> &images, const std::vector<RectPx> &rects) {
  assert(images.size() == rects.size());
  
  PROFILE(blitImages);
  
  Logger::get() << "Copying smaller images onto larger image\n";
  
  if (images.size() == 0) {
    return;
  }
  
  const Image::Format imageFormat = image.format();
  for (auto i = images.cbegin(); i != images.cend(); i++) {
    if (i->format() != imageFormat) {
      throw FormatError();
    }
  }

  for (size_t i = 0; i != images.size(); i++) {
    blit(image, images[i], {rects[i].x, rects[i].y});
  }
}

Image makeAndBlit(const std::vector<Image> &images, const std::vector<RectPx> &rects, const CoordPx length) {
  Image image = makeBlitDst(length, images.size() ? images.front().format() : Image::Format::GREY);
  blitImages(image, images, rects);
  return image;
}
