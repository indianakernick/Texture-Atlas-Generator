//
//  terminal color.hpp
//  Game Engine
//
//  Created by Indi Kernick on 27/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_utils_terminal_color_hpp
#define engine_utils_terminal_color_hpp

#include <cstdio>
#include <cstdint>
#include <cassert>

//ANSI escape codes!

namespace Term {
  enum class Color : uint8_t {
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
  };
  
  void textColor(Color);
  void textColor(uint8_t, uint8_t, uint8_t);
  void defaultTextColor();
  void backColor(Color);
  void backColor(uint8_t, uint8_t, uint8_t);
  void defaultBackColor();
}

#endif
