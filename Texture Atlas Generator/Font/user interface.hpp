//
//  user interface.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 3/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef font_user_interface_hpp
#define font_user_interface_hpp

#include <string>
#include "face.hpp"

void createFontAtlas(
  const std::string &,
  const std::string &,
  const std::vector<FaceSize> &,
  CodePointRange,
  SizePx
);

#endif
