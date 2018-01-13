//
//  unpacker.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 27/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "unpacker.hpp"

#include <fstream>
#include <json.hpp>

inline Unpack::AtlasReadError::AtlasReadError(const char *const msg)
  : std::runtime_error(std::string("Atlas read error: ") + msg) {}

inline Unpack::AtlasReadError::AtlasReadError(const std::exception &exception)
  : std::runtime_error(exception.what()) {}

inline Unpack::SpriteID Unpack::Spritesheet::getIDfromName(const std::string_view name) const {
  auto iter = spriteNames.find(std::string(name));
  if (iter == spriteNames.cend()) {
    return NULL_SPRITE;
  } else {
    return iter->second;
  }
}

inline Unpack::RectPx Unpack::Spritesheet::getSprite(const SpriteID sprite) const {
  return sprites.at(sprite);
}

inline bool Unpack::Spritesheet::hasWhitepixel() const {
  return whitepixel.x != NO_WHITEPIXEL.x || whitepixel.y != NO_WHITEPIXEL.y;
}

inline Unpack::VecPx Unpack::Spritesheet::getWhitepixel() const {
  return whitepixel;
}

inline Unpack::VecPx Unpack::Spritesheet::getSize() const {
  return size;
}

namespace Unpack {
  inline void from_json(const nlohmann::json &json, VecPx &vec) {
    vec = {
      json.at(0).get<CoordPx>(),
      json.at(1).get<CoordPx>()
    };
  }

  inline void from_json(const nlohmann::json &json, RectPx &rect) {
    rect = {
      json.at(0).get<CoordPx>(),
      json.at(1).get<CoordPx>(),
      json.at(2).get<CoordPx>(),
      json.at(3).get<CoordPx>()
    };
  }
}

inline Unpack::Spritesheet Unpack::makeSpritesheet(const std::string_view atlasPath) try {
  std::ifstream atlasFile(atlasPath.data(), std::fstream::binary);
  if (!atlasFile.is_open()) {
    throw AtlasReadError("Failed to open file");
  }
  nlohmann::json doc;
  atlasFile >> doc;
  Spritesheet sheet;
  
  sheet.size = doc.at("size").get<VecPx>();
  if (sheet.size.x < 1 || sheet.size.y < 1) {
    throw AtlasReadError("Size is out of range");
  }
  
  if (const auto whitepixelIter = doc.find("whitepixel"); whitepixelIter != doc.end()) {
    sheet.whitepixel = whitepixelIter->get<VecPx>();
    if (sheet.whitepixel.x < 0 || sheet.whitepixel.y < 0) {
      throw AtlasReadError("Whitepixel is out of range");
    }
  } else {
    sheet.whitepixel = Spritesheet::NO_WHITEPIXEL;
  }
  
  sheet.sprites = doc.at("rects").get<decltype(sheet.sprites)>();
  sheet.spriteNames = doc.at("names").get<decltype(sheet.spriteNames)>();
  
  if (sheet.sprites.size() != sheet.spriteNames.size()) {
    throw AtlasReadError("There must be one name for each rectagle");
  }
  
  for (const RectPx rect : sheet.sprites) {
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
  }
  
  return sheet;
} catch (AtlasReadError &) {
  throw;
} catch (std::exception &e) {
  throw AtlasReadError(e);
}
