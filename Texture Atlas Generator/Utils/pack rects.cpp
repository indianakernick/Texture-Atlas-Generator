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

SizePx calcArea(const std::vector<RectPx> &rects, const SizePx sep) {
  SizePx area = 0;
  for (auto i = rects.cbegin(); i != rects.cend(); ++i) {
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

std::vector<stbrp_rect> fillStbRects(const std::vector<RectPx> &rects, const SizePx sep) {
  PROFILE(fillRects);
  
  std::vector<stbrp_rect> stbRects(rects.size());
  
  for (size_t i = 0; i != rects.size(); i++) {
    stbRects[i].id = static_cast<int>(i);
    stbRects[i].w = static_cast<stbrp_coord>(rects[i].s.x + sep);
    stbRects[i].h = static_cast<stbrp_coord>(rects[i].s.y + sep);
    stbRects[i].was_packed = 0;
  }
  
  return stbRects;
}

std::vector<stbrp_rect> packRects(
  const std::vector<RectPx> &rects,
  const SizePx length,
  const SizePx sep
) {
  PROFILE(packRects helper);

  std::vector<stbrp_node> stbNodes(length);
  std::vector<stbrp_rect> stbRects = fillStbRects(rects, sep);
  
  stbrp_context ctx;
  stbrp_init_target(&ctx, length, length, stbNodes.data(), static_cast<int>(stbNodes.size()));
  if (stbrp_pack_rects(&ctx, stbRects.data(), static_cast<int>(stbRects.size())) == 0) {
    throw RectPackError();
  }
  
  return stbRects;
}

SizePx packRects(std::vector<RectPx> &rects, const SizePx sep) {
  PROFILE(packRects);

  std::cout << "Packing rectangles\n";
  
  if (rects.size() == 0) {
    return 0;
  }
  
  const SizePx length = calcLength(calcArea(rects, sep));
  std::vector<stbrp_rect> stbRects = packRects(rects, length, sep);
  
  for (size_t i = 0; i != rects.size(); i++) {
    rects[i].p.x = stbRects[i].x;
    rects[i].p.y = stbRects[i].y;
  }
  
  return length;
}
