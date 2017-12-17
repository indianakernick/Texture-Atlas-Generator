//
//  write atlas.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 4/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "write atlas.hpp"

#include <fstream>
#include "../Utils/logger.hpp"
#include "../Utils/profiler.hpp"

static const VecPx NO_WHITEPIXEL = {-1, -1};

AtlasWriteError::AtlasWriteError(const char *msg)
  : std::runtime_error(std::string("Failed to write atlas: ") + msg) {}

AtlasWriteError::AtlasWriteError(const std::string &msg)
  : std::runtime_error("Failed to write atlas: " + msg) {}

AtlasWriteError::AtlasWriteError(const std::exception &exception)
  : std::runtime_error(exception.what()) {}

std::string_view getImageName(const std::string &path) {
  const size_t lastSlash = path.find_last_of('/');
  return {path.c_str() + lastSlash + 1, path.find_last_of('.') - lastSlash - 1};
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
  const std::string_view output,
  const std::vector<std::string> &paths,
  const std::vector<RectPx> &rects,
  const CoordPx size,
  const bool hasWhitepixel
) try {
  PROFILE(writeAtlas);

  Logger::get() << "Writing atlas to file \"" << output << "\"\n";
  
  std::ofstream file(output.data(), std::fstream::binary);
  if (!file.is_open()) {
    throw AtlasWriteError("Could not open output file");
  }
  file.exceptions(std::fstream::eofbit | std::fstream::failbit | std::fstream::badbit);
  
  const VecPx sizeVec = {size, size};
  file.write(reinterpret_cast<const char *>(&sizeVec), sizeof(sizeVec));
  
  const VecPx whitepixel = getWhitepixel(rects.back(), hasWhitepixel);
  file.write(reinterpret_cast<const char *>(&whitepixel), sizeof(whitepixel));
  
  const CoordPx numSprites = static_cast<CoordPx>(paths.size());
  file.write(reinterpret_cast<const char *>(&numSprites), sizeof(numSprites));
  
  file.write(reinterpret_cast<const char *>(rects.data()), (rects.size() - hasWhitepixel) * sizeof(RectPx));
  
  std::vector<std::string_view> names;
  for (auto p = paths.cbegin(); p != paths.cend(); ++p) {
    const std::string_view name = getImageName(*p);
    for (auto n = names.cbegin(); n != names.cend(); ++n) {
      if (*n == name) {
        throw AtlasWriteError("Two images have the same name \"" + std::string(name) + "\"");
      }
    }
    names.push_back(name);
  }
  
  for (auto n = names.cbegin(); n != names.cend(); ++n) {
    file.write(n->data(), n->size());
    file.put(0);
  }
  
  file.flush();
} catch (AtlasWriteError &) {
  throw;
} catch (std::exception &e) {
  throw AtlasWriteError(e);
}
