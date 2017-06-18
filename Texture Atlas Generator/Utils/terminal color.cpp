//
//  terminal color.cpp
//  Game Engine
//
//  Created by Indi Kernick on 27/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "terminal color.hpp"

#define ESC "\x1B["

void Term::cursorUp(unsigned n) {
  std::printf(ESC "%uA", n);
}

void Term::cursorDown(unsigned n) {
  std::printf(ESC "%uB", n);
}

void Term::cursorForward(unsigned n) {
  std::printf(ESC "%uC", n);
}

void Term::cursorBack(unsigned n) {
  std::printf(ESC "%uD", n);
}

void Term::cursorNextLine(unsigned n) {
  std::printf(ESC "%uE", n);
}

void Term::cursorPrevLine(unsigned n) {
  std::printf(ESC "%uF", n);
}

void Term::cursorHoriPos(unsigned col) {
  std::printf(ESC "%uG", col);
}

void Term::cursorPos(unsigned row, unsigned col) {
  std::printf(ESC "%u;%uG", row, col);
}

Term::CursorPos Term::getCursorPos() {
  std::printf(ESC "6n");
  CursorPos pos;
  std::scanf(ESC "%u;%uR", &pos.row, &pos.col);
  return pos;
}

void Term::saveCursor() {
  std::printf(ESC "s");
}

void Term::restoreCursor() {
  std::printf(ESC "u");
}

void Term::hideCursor() {
  std::printf(ESC "?25l");
}

void Term::showCursor() {
  std::printf(ESC "?25h");
}

void Term::scrollUp(unsigned n) {
  std::printf(ESC "%uS", n);
}

void Term::scrollDown(unsigned n) {
  std::printf(ESC "%uT", n);
}

void Term::eraseDisplay(Clear clear) {
  std::printf(ESC "%cJ", static_cast<uint8_t>(clear));
}

void Term::eraseLine(Clear clear) {
  std::printf(ESC "%cK", static_cast<uint8_t>(clear));
}

void Term::videoReset() {
  std::printf(ESC "0m");
}

void Term::intensity(Intensity intensity) {
  switch (intensity) {
    case Intensity::NORMAL:
      std::printf(ESC "22m");
      break;
    case Intensity::BOLD:
      std::printf(ESC "1m");
      break;
    case Intensity::FAINT:
      std::printf(ESC "2m");
  }
}

void Term::italic(bool on) {
  if (on) {
    std::printf(ESC "3m");
  } else {
    std::printf(ESC "23m");
  }
}

void Term::underline(bool on) {
  if (on) {
    std::printf(ESC "4m");
  } else {
    std::printf(ESC "24m");
  }
}

void Term::blink(Blink rate) {
  switch (rate) {
    case Blink::OFF:
      std::printf(ESC "25m");
      break;
    case Blink::SLOW:
      std::printf(ESC "5m");
      break;
    case Blink::FAST:
      std::printf(ESC "6m");
  }
}

void Term::videoNegative(bool on) {
  if (on) {
    std::printf(ESC "7m");
  } else {
    std::printf(ESC "27m");
  }
}

void Term::conceal(bool on) {
  if (on) {
    std::printf(ESC "8m");
  } else {
    std::printf(ESC "28m");
  }
}

void Term::strikethrough(bool on) {
  if (on) {
    std::printf(ESC "9m");
  } else {
    std::printf(ESC "29m");
  }
}

void Term::primaryFont() {
  std::printf(ESC "10m");
}

void Term::alternativeFont(unsigned n) {
  assert(n < 9);
  std::printf(ESC "%um", 11 + n);
}

void Term::textColor(Color color) {
  std::printf(ESC "%um", 30u + static_cast<unsigned>(color));
}

void Term::textColor(uint8_t r, uint8_t g, uint8_t b) {
  std::printf(ESC "38;2;%c;%c;%cm", r, g, b);
}

void Term::defaultTextColor() {
  std::printf(ESC "39m");
}

void Term::backColor(Color color) {
  std::printf(ESC "%um", 40u + static_cast<unsigned>(color));
}

void Term::backColor(uint8_t r, uint8_t g, uint8_t b) {
  std::printf(ESC "48;2;%c;%c;%cm", r, g, b);
}

void Term::defaultBackColor() {
  std::printf(ESC "49m");
}

#undef ESC
