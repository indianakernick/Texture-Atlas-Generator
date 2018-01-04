//
//  user interface.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 3/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "user interface.hpp"

#include "load images.hpp"
#include "blit images.hpp"
#include "write image.hpp"
#include "write atlas.hpp"
#include "sort by frame.hpp"
#include "rects from images.hpp"
#include "../Utils/pack rects.hpp"
#include "../Utils/search dir.hpp"

const std::string DEFAULT_INPUT = ".";
const std::string DEFAULT_OUTPUT = "output";
const CoordPx DEFAULT_SEP = 1;

using nlohmann::json;

template <typename ValueType>
ValueType getOptional(const json &node, const char *key, const ValueType &def) {
  const auto iter = node.find(key);
  if (iter == node.end()) {
    return def;
  } else {
    return iter->get<ValueType>();
  }
}

void createImageAtlas(const json &config) {
  const std::string inputFolder = getOptional(config, "input", DEFAULT_INPUT);
  const std::string outputName = getOptional(config, "output", DEFAULT_OUTPUT);
  const CoordPx sep = getOptional(config, "sep", DEFAULT_SEP);
  const auto whitepixelIter = config.find("whitepixel");
  const bool recursive = getOptional(config, "recursive", false);
  const size_t maxDepth = getOptional(
    config,
    "max resursive depth",
    std::numeric_limits<size_t>::max()
  );

  std::remove((outputName + ".png").c_str());
  std::vector<std::string> paths(
    recursive
    ? findFilesRec(inputFolder, extIsImage, maxDepth)
    : findFiles(inputFolder, extIsImage)
  );
  sortByFrame(paths);
  
  std::vector<Surface> images = loadImages(paths);
  const bool hasWhitepixel = whitepixelIter != config.end();
  if (hasWhitepixel) {
    const CoordPx size = 1 + whitepixelIter->get<CoordPx>() * 2;
    images.emplace_back(size, size, images.back().bytesPerPixel(), 255);
  }
  std::vector<RectPx> rects = rectsFromImages(images);
  const CoordPx length = packRects(rects, sep);
  
  writeImage(outputName + ".png", makeAndBlit(images, rects, length));
  writeAtlas(outputName + ".atlas", paths, rects, length, hasWhitepixel);
}
