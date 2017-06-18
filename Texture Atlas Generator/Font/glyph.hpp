//
//  glyph.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 24/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef font_glyph_hpp
#define font_glyph_hpp

#include "../Utils/types.hpp"

struct GlyphMetrics {
  PosPx2 bearing;
  SizePx2 size;
  PosPx advance;
};

#endif
