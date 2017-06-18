//
//  write atlas.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 4/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef font_write_atlas_hpp
#define font_write_atlas_hpp

#include "face.hpp"

void writeAtlas(
  const std::string &,
  const std::vector<Face> &,
  const std::vector<RectPx> &,
  SizePx
);

#endif
