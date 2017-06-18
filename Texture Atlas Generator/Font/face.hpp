//
//  face.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 8/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef font_face_hpp
#define font_face_hpp

#include <vector>
#include "types.hpp"
#include "glyph.hpp"
#include "../Image/image.hpp"
#include "../Utils/move only.hpp"

struct FaceSize {
  DPI dpi;
  SizePt points;
};

struct FaceMetrics {
  SizePx lineHeight;
  PosPx minY;
  PosPx maxY;
};

struct Face {
  MOVE_ONLY(Face);
  
  std::vector<PosPx> kerning;
  std::vector<GlyphMetrics> glyphMetrics;
  std::vector<Image> glyphs;
  FaceMetrics faceMetrics;
  FaceSize size;
  CodePointRange range;
};

#endif
