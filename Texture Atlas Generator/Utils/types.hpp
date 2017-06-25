//
//  types.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 6/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef utils_types_hpp
#define utils_types_hpp

#include "vector.hpp"

//These types are written to a file so they're size needs to be constant
using PosPx = int32_t;
using SizePx = uint32_t;
using PosPx2 = tvec2<PosPx>;
using SizePx2 = tvec2<SizePx>;

#pragma pack(1)
struct RectPx {
  PosPx2 p;
  SizePx2 s;
};
#pragma pack()

#endif
