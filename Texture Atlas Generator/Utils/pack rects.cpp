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

CoordPx calcArea(const std::vector<RectPx> &rects, const CoordPx sep) {
  CoordPx area = 0;
  for (auto i = rects.cbegin(); i != rects.cend(); ++i) {
    area += (i->w + sep) * (i->h + sep);
  }
  return area;
}

CoordPx calcLength(CoordPx area) {
  const CoordPx length = static_cast<CoordPx>(std::ceil(std::sqrt(area)));
  const CoordPx ceiledLength = ceilToPowerOf2(length);
  if (static_cast<float>(length) / ceiledLength > 0.90f) {
    return ceiledLength * 2;
  } else {
    return ceiledLength;
  }
}

std::vector<stbrp_rect> fillStbRects(const std::vector<RectPx> &rects, const CoordPx sep) {
  PROFILE(fillRects);
  
  std::vector<stbrp_rect> stbRects(rects.size());
  
  for (size_t i = 0; i != rects.size(); i++) {
    stbRects[i].id = static_cast<int>(i);
    stbRects[i].w = static_cast<stbrp_coord>(rects[i].w + sep);
    stbRects[i].h = static_cast<stbrp_coord>(rects[i].h + sep);
    stbRects[i].was_packed = 0;
  }
  
  return stbRects;
}

std::vector<stbrp_rect> packRects(
  const std::vector<RectPx> &rects,
  const CoordPx length,
  const CoordPx sep
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

CoordPx packRects(std::vector<RectPx> &rects, const CoordPx sep) {
  PROFILE(packRects);

  std::cout << "Packing rectangles\n";
  
  if (rects.size() == 0) {
    return 0;
  }
  
  const CoordPx length = calcLength(calcArea(rects, sep));
  std::vector<stbrp_rect> stbRects = packRects(rects, length, sep);
  
  for (size_t i = 0; i != rects.size(); i++) {
    rects[i].x = stbRects[i].x;
    rects[i].y = stbRects[i].y;
  }
  
  return length;
}
