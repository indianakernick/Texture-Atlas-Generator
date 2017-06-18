//
//  write atlas.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 4/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef image_write_atlas_hpp
#define image_write_atlas_hpp

#include <vector>
#include "image.hpp"

void writeAtlas(
  const std::string &,
  const std::vector<std::string> &,
  const std::vector<RectPx> &,
  SizePx,
  bool
);

#endif
