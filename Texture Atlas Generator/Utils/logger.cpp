//
//  logger.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 30/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "logger.hpp"

std::streamsize SilentBuf::xsputn(const char_type *, const std::streamsize n) {
  return n;
}

SilentBuf::int_type SilentBuf::overflow(const int_type c) {
  return c;
}

std::unique_ptr<std::ostream> Logger::stream;

void Logger::init(const LogOutput output) {
  if (output == LogOutput::SILENT) {
    static std::unique_ptr<SilentBuf> buf = std::make_unique<SilentBuf>();
    stream = std::make_unique<std::ostream>(buf.get());
  } else if (output == LogOutput::COUT) {
    stream = std::make_unique<std::ostream>(std::cout.rdbuf());
  }
}

std::ostream &Logger::get() {
  return *stream.get();
}
