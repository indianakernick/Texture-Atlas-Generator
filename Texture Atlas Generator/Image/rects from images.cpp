//
//  rects from images.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 11/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "rects from images.hpp"

std::vector<RectPx> rectsFromImages(const std::vector<Image> &images) {
  std::vector<RectPx> rects;
  rectsFromImages(images, rects);
  return rects;
}

void rectsFromImages(const std::vector<Image> &images, std::vector<RectPx> &rects) {
  rects.reserve(rects.size() + images.size());
  for (auto i = images.cbegin(); i != images.cend(); i++) {
    rects.push_back({{0, 0}, i->s});
  }
}
