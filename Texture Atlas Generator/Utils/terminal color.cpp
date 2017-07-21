//
//  terminal color.cpp
//  Game Engine
//
//  Created by Indi Kernick on 27/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "terminal color.hpp"

#include <cstdio>

#define ESC "\x1B["

void Term::textColor(const Color color) {
  std::printf(ESC "%um", 30u + static_cast<unsigned>(color));
}

void Term::textColor(const uint8_t r, const uint8_t g, const uint8_t b) {
  std::printf(ESC "38;2;%c;%c;%cm", r, g, b);
}

void Term::defaultTextColor() {
  std::printf(ESC "39m");
}

void Term::backColor(const Color color) {
  std::printf(ESC "%um", 40u + static_cast<unsigned>(color));
}

void Term::backColor(const uint8_t r, const uint8_t g, const uint8_t b) {
  std::printf(ESC "48;2;%c;%c;%cm", r, g, b);
}

void Term::defaultBackColor() {
  std::printf(ESC "49m");
}

#undef ESC
