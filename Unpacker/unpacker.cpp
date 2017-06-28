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
#include "load images.hpp"

const VecPx Spritesheet::NO_WHITEPIXEL = {-1, -1};

SpriteNotFound::SpriteNotFound()
  : std::range_error("Sprite was not found") {}

AtlasReadError::AtlasReadError(const char *msg)
  : std::runtime_error(std::string("Atlas read error: ") + msg) {}

AtlasReadError::AtlasReadError(const std::exception &exception)
  : std::runtime_error(exception.what()) {}

Spritesheet::Spritesheet()
  : image(1, 1, Image::Format::GREY, 0, nullptr, noDelete) {}

bool Spritesheet::hasWhitepixel() const {
  return whitepixel != NO_WHITEPIXEL;
}

VecPx Spritesheet::getWhitepixel() const {
  return whitepixel;
}

RectPx Spritesheet::getSprite(const std::string &name) const {
  auto iter = sprites.find(name);
  if (iter == sprites.end()) {
    throw SpriteNotFound();
  } else {
    return iter->second;
  }
}

const Image &Spritesheet::getImage() const {
  return image;
}

Spritesheet::Spritesheet(Image &&image)
  : image(std::move(image)) {}

template <typename DataType>
DataType read(std::ifstream &file) {
  DataType data;
  file.read(reinterpret_cast<std::ifstream::char_type *>(&data), sizeof(data));
  return data;
}

Spritesheet makeSpritesheet(const std::string &atlasPath, const std::string &imagePath) try {
  std::ifstream atlasFile(atlasPath, std::fstream::binary);
  if (!atlasFile.is_open()) {
    throw AtlasReadError("Failed to open file");
  }
  atlasFile.exceptions(std::fstream::eofbit | std::fstream::failbit | std::fstream::badbit);
  Spritesheet sheet(loadImage(imagePath));
  
  const VecPx size = read<VecPx>(atlasFile);
  if (size.x < 1 || size.y < 1) {
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
  
  std::vector<RectPx> rectangles;
  rectangles.reserve(numSprites);
  
  while (numSprites--) {
    const RectPx rect = read<RectPx>(atlasFile);
    if (
      rect.x < 0 ||
      rect.y < 0 ||
      rect.w < 1 ||
      rect.h < 1 ||
      rect.x + rect.w < size.x ||
      rect.y + rect.h < size.y
    ) {
      throw AtlasReadError("Rectangle out of range");
    }
    rectangles.push_back(rect);
  }
  
  for (size_t s = 0; s != rectangles.size(); s++) {
    std::string name;
    while (char c = atlasFile.get()) {
      name.push_back(c);
    }
    const bool inserted = sheet.sprites.emplace(name, rectangles[s]).second;
    if (not inserted) {
      throw AtlasReadError("More than one sprite have the same name");
    }
  }
  
  return sheet;
} catch (AtlasReadError &) {
  throw;
} catch (std::exception &e) {
  throw AtlasReadError(e);
}
