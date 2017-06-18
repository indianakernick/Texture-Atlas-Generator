//
//  load font.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 24/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef font_load_font_hpp
#define font_load_font_hpp

#include <string>
#include "font.hpp"

class FontLoadError final : public std::runtime_error {
public:
  FontLoadError(const std::string &, const char *);
};

Font loadFont(const std::string &);

#endif
