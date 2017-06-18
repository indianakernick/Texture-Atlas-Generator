//
//  user interface.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 3/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef image_user_interface_hpp
#define image_user_interface_hpp

#include <string>
#include "types.hpp"

const SizePx NO_WHITE_PIXEL = std::numeric_limits<SizePx>::max();

void createImageAtlas(
  const std::string &,
  const std::string &,
  SizePx,
  SizePx
);

#endif
