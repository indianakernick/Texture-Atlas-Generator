//
//  pack rects.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "pack rects.hpp"

#include <cmath>
#include <iostream>
#include "math.hpp"
#include "profiler.hpp"
#include "../Libraries/stb_rect_pack.h"

RectPackError::RectPackError()
  : std::runtime_error("Failed to pack rectangles") {}

SizePx calcArea(const std::vector<RectPx> &rects, SizePx sep) {
  SizePx area = 0;
  for (auto i = rects.cbegin(); i != rects.cend(); i++) {
    area += (i->s.x + sep) * (i->s.y + sep);
  }
  return area;
}

SizePx calcLength(SizePx area) {
  const SizePx length = static_cast<SizePx>(std::ceil(std::sqrt(area)));
  const SizePx ceiledLength = ceilToPowerOf2(length);
  if (static_cast<float>(length) / ceiledLength > 0.90f) {
    return ceiledLength * 2;
  } else {
    return ceiledLength;
  }
}

std::vector<stbrp_rect> fillLibRects(const std::vector<RectPx> &rects, SizePx sep) {
  PROFILE(fillRects);
  
  std::vector<stbrp_rect> libRects(rects.size());
  
  for (size_t i = 0; i != rects.size(); i++) {
    libRects[i].id = static_cast<int>(i);
    libRects[i].w = static_cast<stbrp_coord>(rects[i].s.x + sep);
    libRects[i].h = static_cast<stbrp_coord>(rects[i].s.y + sep);
    libRects[i].was_packed = 0;
  }
  
  return libRects;
}

void checkAllRectsPacked(const std::vector<stbrp_rect> &rects) {
  PROFILE(checkAllRectsPacked);

  for (size_t r = 0; r != rects.size(); r++) {
    if (rects[r].was_packed == 0) {
      throw RectPackError();
    }
  }
}

std::vector<stbrp_rect> packRects(
  const std::vector<RectPx> &rects,
  SizePx length,
  SizePx sep
) {
  PROFILE(packRects helper);

  std::vector<stbrp_node> libNodes(length);
  std::vector<stbrp_rect> libRects = fillLibRects(rects, sep);
  
  stbrp_context ctx;
  stbrp_init_target(&ctx, length, length, libNodes.data(), static_cast<int>(libNodes.size()));
  stbrp_pack_rects(&ctx, libRects.data(), static_cast<int>(libRects.size()));
  
  checkAllRectsPacked(libRects);
  
  return libRects;
}

SizePx packRects(std::vector<RectPx> &rects, SizePx sep) {
  PROFILE(packRects);

  std::cout << "Packing rectangles\n";
  
  const SizePx length = calcLength(calcArea(rects, sep));
  std::vector<stbrp_rect> libRects = packRects(rects, length, sep);
  
  for (size_t i = 0; i != rects.size(); i++) {
    rects[i].p.x = libRects[i].x;
    rects[i].p.y = libRects[i].y;
  }
  
  return length;
}
