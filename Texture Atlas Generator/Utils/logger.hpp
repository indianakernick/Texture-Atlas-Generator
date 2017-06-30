//
//  logger.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 30/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef utils_logger_hpp
#define utils_logger_hpp

#include <iostream>

class SilentBuf : public std::streambuf {
public:
  SilentBuf() = default;

  std::streamsize xsputn(const char_type* s, std::streamsize n) override;
  int_type overflow(int_type c) override;
};

enum class LogOutput {
  SILENT,
  COUT
};

class Logger {
public:
  Logger() = delete;
  ~Logger() = delete;
  
  static void init(LogOutput);
  static std::ostream &get();

private:
  static std::unique_ptr<std::ostream> stream;
};

#endif
