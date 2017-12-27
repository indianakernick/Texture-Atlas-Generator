//
//  blit images.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 23/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "blit images.hpp"

#include <cassert>
#include <Surface/blit.hpp>
#include "../Utils/logger.hpp"

Surface makeBlitDst(const CoordPx length, const Surface::BytesPerPixel format) {
  return Surface(length, length, format, 0);
}

void blitImages(Surface &image, const std::vector<Surface> &images, const std::vector<RectPx> &rects) {
  assert(images.size() == rects.size());
  
  Logger::get() << "Copying smaller images onto larger image\n";
  
  if (images.size() == 0) {
    return;
  }
  
  const Surface::BytesPerPixel imageFormat = image.bytesPerPixel();
  for (auto i = images.cbegin(); i != images.cend(); i++) {
    if (i->bytesPerPixel() != imageFormat) {
      throw FormatError();
    }
  }

  for (size_t i = 0; i != images.size(); i++) {
    blit(image, images[i], rects[i].x, rects[i].y);
  }
}

Surface makeAndBlit(const std::vector<Surface> &images, const std::vector<RectPx> &rects, const CoordPx length) {
  Surface image = makeBlitDst(length, images.size() ? images.front().bytesPerPixel() : 1);
  blitImages(image, images, rects);
  return image;
}
