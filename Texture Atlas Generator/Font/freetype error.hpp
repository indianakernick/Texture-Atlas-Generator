//
//  freetype error.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 30/3/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef font_freetype_error_hpp
#define font_freetype_error_hpp

#include <ft2build.h>
#include FT_TYPES_H
#include <stdexcept>

class FreeTypeError final : public std::runtime_error {
public:
  explicit FreeTypeError(const char *);
};

const char *getFTErrorString(FT_Error);

#define CHECK_FT_ERROR(expression) \
  if (const FT_Error error = (expression)) { \
    throw FreeTypeError(getFTErrorString(error)); \
  }

#endif
