//
//  pack rects.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef utils_pack_rects_hpp
#define utils_pack_rects_hpp

#include <vector>
#include "types.hpp"

class RectPackError final : public std::runtime_error {
public:
  RectPackError();
};

SizePx packRects(std::vector<RectPx> &, SizePx);

#endif
