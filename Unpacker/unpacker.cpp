//
//  unpacker.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 27/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "unpacker.hpp"

#include <vector>
#include <fstream>
#include <Surface/load.hpp>

using namespace Unpack;

const VecPx Spritesheet::NO_WHITEPIXEL = {-1, -1};

AtlasReadError::AtlasReadError(const char *msg)
  : std::runtime_error(std::string("Atlas read error: ") + msg) {}

AtlasReadError::AtlasReadError(const std::exception &exception)
  : std::runtime_error(exception.what()) {}

SpriteID Spritesheet::getIDfromName(const std::string_view name) const {
  auto iter = spriteNames.find(std::string(name));
  if (iter == spriteNames.cend()) {
    return NULL_SPRITE;
  } else {
    return iter->second;
  }
}

RectPx Spritesheet::getSprite(const SpriteID sprite) const {
  return sprites.at(sprite);
}

bool Spritesheet::hasWhitepixel() const {
  return whitepixel.x != NO_WHITEPIXEL.x || whitepixel.y != NO_WHITEPIXEL.y;
}

VecPx Spritesheet::getWhitepixel() const {
  return whitepixel;
}

VecPx Spritesheet::getSize() const {
  return size;
}

template <typename DataType>
DataType read(std::ifstream &file) {
  DataType data;
  file.read(reinterpret_cast<std::ifstream::char_type *>(&data), sizeof(data));
  return data;
}

Spritesheet Unpack::makeSpritesheet(const std::string_view atlasPath) try {
  std::ifstream atlasFile(atlasPath.data(), std::fstream::binary);
  if (!atlasFile.is_open()) {
    throw AtlasReadError("Failed to open file");
  }
  atlasFile.exceptions(0xFFFFFFFF);
  Spritesheet sheet;
  
  sheet.size = read<VecPx>(atlasFile);
  if (sheet.size.x < 1 || sheet.size.y < 1) {
    throw AtlasReadError("Size is out of range");
  }
  
  sheet.whitepixel = read<VecPx>(atlasFile);
  if (sheet.whitepixel.x < -1 || sheet.whitepixel.y < -1) {
    throw AtlasReadError("Whitepixel is out of range");
  }
  
  CoordPx numSprites = read<CoordPx>(atlasFile);
  if (numSprites < 1) {
    throw AtlasReadError("Number of sprites is out of range");
  }
  
  sheet.sprites.reserve(numSprites);
  
  while (numSprites--) {
    const RectPx rect = read<RectPx>(atlasFile);
    if (
      rect.x < 0 ||
      rect.y < 0 ||
      rect.w < 1 ||
      rect.h < 1 ||
      rect.x + rect.w > sheet.size.x ||
      rect.y + rect.h > sheet.size.y
    ) {
      throw AtlasReadError("Rectangle out of range");
    }
    sheet.sprites.push_back(rect);
  }
  
  for (size_t s = 0; s != sheet.sprites.size(); ++s) {
    std::string name;
    while (const char c = atlasFile.get()) {
      name.push_back(c);
    }
    const bool inserted = sheet.spriteNames.emplace(name, s).second;
    if (!inserted) {
      throw AtlasReadError("More than one sprite have the same name");
    }
  }
  
  return sheet;
} catch (AtlasReadError &) {
  throw;
} catch (std::exception &e) {
  throw AtlasReadError(e);
}
