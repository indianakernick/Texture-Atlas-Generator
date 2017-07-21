//
//  write atlas.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 4/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "write atlas.hpp"

#include <cstdio>
#include "../Utils/logger.hpp"
#include "../Utils/profiler.hpp"

static const VecPx NO_WHITEPIXEL = {-1, -1};

AtlasWriteError::AtlasWriteError(const char *msg)
  : std::runtime_error(std::string("Failed to write atlas: ") + msg) {}

AtlasWriteError::AtlasWriteError(const std::string &msg)
  : std::runtime_error("Failed to write atlas: " + msg) {}

std::experimental::string_view getImageName(const std::string &path) {
  const size_t lastSlash = path.find_last_of('/');
  return {path.c_str() + lastSlash + 1, path.find_last_of('.') - lastSlash - 1};
}

using File = std::unique_ptr<std::FILE, int(*)(std::FILE *)>;

File openFile(std::experimental::string_view path) {
  std::FILE *const file = std::fopen(path.data(), "wb");
  if (file == nullptr) {
    throw AtlasWriteError("Could not open output file");
  } else {
    return {file, std::fclose};
  }
}

void write(std::FILE *const file, const void *ptr, const size_t size) {
  if (std::fwrite(ptr, size, 1, file) == 0) {
    throw AtlasWriteError("Could not write to file");
  }
}

VecPx getWhitepixel(const RectPx lastRect, const bool hasWhitepixel) {
  if (hasWhitepixel) {
    return {
      lastRect.x + (lastRect.w - 1) / 2,
      lastRect.y + (lastRect.h - 1) / 2
    };
  } else {
    return NO_WHITEPIXEL;
  }
}

/*
0 ~ 8       Width and height of sheet
8 ~ 16      X and Y of whitepixel
16 ~ 20     Number of rectangles
20 ~ 20+n*4 Rectangles X, Y, W, H
20+n*4 +    Null-terminated ASCII string names
*/

void writeAtlas(
  const std::experimental::string_view output,
  const std::vector<std::string> &paths,
  const std::vector<RectPx> &rects,
  const CoordPx size,
  const bool hasWhitepixel
) {
  PROFILE(writeAtlas);

  Logger::get() << "Writing atlas to file \"" << output << "\"\n";
  
  File file = openFile(output);
  
  const VecPx sizeVec = {size, size};
  write(file.get(), &sizeVec, sizeof(sizeVec));
  
  const VecPx whitepixel = getWhitepixel(rects.back(), hasWhitepixel);
  write(file.get(), &whitepixel, sizeof(whitepixel));
  
  const CoordPx numSprites = static_cast<CoordPx>(paths.size());
  write(file.get(), &numSprites, sizeof(numSprites));
  
  write(file.get(), rects.data(), (rects.size() - hasWhitepixel) * sizeof(RectPx));
  
  std::vector<std::experimental::string_view> names;
  for (auto p = paths.cbegin(); p != paths.cend(); ++p) {
    const std::experimental::string_view name = getImageName(*p);
    for (auto n = names.cbegin(); n != names.cend(); ++n) {
      if (*n == name) {
        throw AtlasWriteError("Two images have the same name \"" + name.to_string() + "\"");
      }
    }
    names.push_back(name);
  }
  
  for (auto n = names.cbegin(); n != names.cend(); ++n) {
    write(file.get(), n->data(), n->size());
    if (std::fputc(0, file.get()) == EOF) {
      throw AtlasWriteError("Could not write null terminator");
    }
  }
  
  std::fflush(file.get());
}
