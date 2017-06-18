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

using PosPx = int;
using SizePx = unsigned;
using PosPx2 = tvec2<PosPx>;
using SizePx2 = tvec2<SizePx>;

struct RectPx {
  PosPx2 p;
  SizePx2 s;
};

#endif
