//
//  image.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "image.hpp"

#include <cstdlib>
#include <cassert>

#if !defined(BUILDING_PACKER)
using namespace Unpack;
void Unpack::defaultDelete(void *ptr) {
  std::free(ptr);
}

void Unpack::noDelete(void *) {}
#else
void defaultDelete(void *ptr) {
  std::free(ptr);
}

void noDelete(void *) {}
#endif

uint8_t *defaultNew(const CoordPx width, const CoordPx height, const Image::Format format) {
  assert(width > 0);
  assert(height > 0);
  return reinterpret_cast<uint8_t *>(
    std::malloc(width * height * static_cast<CoordPx>(format))
  );
}

//stb_image_write calls realloc on the pointer stored in the image
//so the memory has to be allocated with malloc

Image::Image(const CoordPx width, const CoordPx height, const Format format)
  : mData(defaultNew(width, height, format), defaultDelete),
    mPitch(width * static_cast<CoordPx>(format)),
    mWidth(width),
    mHeight(height),
    mFormat(format) {
  assert(width > 0);
  assert(height > 0);
}

Image::Image(const CoordPx width, const CoordPx height, const Format format, const uint8_t byte)
  : Image(width, height, format) {
  std::memset(mData.get(), byte, height * mPitch);
}

Image::Image(
  const CoordPx width,
  const CoordPx height,
  const Format format,
  const ptrdiff_t pitch,
  uint8_t *const data,
  const Deleter deleter
) : mData(data, deleter),
    mPitch(pitch),
    mWidth(width),
    mHeight(height),
    mFormat(format) {
  assert(width > 0);
  assert(height > 0);
}

uint8_t *Image::data() {
  return mData.get();
}

uint8_t *Image::data(const VecPx pos) {
  return mData.get() + (pos.y * mPitch + pos.x * static_cast<ptrdiff_t>(mFormat));
}

uint8_t *Image::dataEnd() {
  return mData.get() + (mHeight * mPitch);
}

const uint8_t *Image::data() const {
  return mData.get();
}

const uint8_t *Image::data(const VecPx pos) const {
  return mData.get() + (pos.y * mPitch + pos.x * static_cast<ptrdiff_t>(mFormat));
}

const uint8_t *Image::dataEnd() const {
  return mData.get() + (mHeight * mPitch);
}

size_t Image::size() const {
  return std::abs(mHeight * mPitch);
}

ptrdiff_t Image::pitch() const {
  return mPitch;
}

ptrdiff_t Image::padding() const {
  return mPitch - widthBytes();
}

CoordPx Image::width() const {
  return mWidth;
}

ptrdiff_t Image::widthBytes() const {
  return mWidth * static_cast<ptrdiff_t>(mFormat);
}

CoordPx Image::height() const {
  return mHeight;
}

Image::Format Image::format() const {
  return mFormat;
}
