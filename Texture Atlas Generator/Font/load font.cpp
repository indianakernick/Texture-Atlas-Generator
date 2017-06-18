//
//  load font.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 24/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "load font.hpp"

#include <iostream>
#include "freetype error.hpp"
#include "../Utils/profiler.hpp"

FontLoadError::FontLoadError(const std::string &file, const char *reason)
  : std::runtime_error("Failed to load font \"" + file + "\": " + reason) {}

FTHandle<FT_Library, FT_Done_FreeType> freetype;

void initFreetype() {
  if (!freetype) {
    CHECK_FT_ERROR(FT_Init_FreeType(&freetype));
  }
}

Font loadFont(const std::string &path) {
  PROFILE(loadFont);

  std::cout << "Loading font \"" << path << "\"\n";
  
  initFreetype();
  Font font;
  CHECK_FT_ERROR(FT_New_Face(freetype, path.c_str(), 0, &font));
  return font;
}
