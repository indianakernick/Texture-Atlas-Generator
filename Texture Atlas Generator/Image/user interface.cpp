//
//  user interface.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 3/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "user interface.hpp"

#include <yaml-cpp/yaml.h>
#include "load images.hpp"
#include "blit images.hpp"
#include "write image.hpp"
#include "write atlas.hpp"
#include "sort by frame.hpp"
#include "rects from images.hpp"
#include "../Utils/profiler.hpp"
#include "../Utils/pack rects.hpp"
#include "../Utils/search dir.hpp"

static const std::string DEFAULT_INPUT = ".";
static const std::string DEFAULT_OUTPUT = "output";
static const CoordPx DEFAULT_SEP = 1;

template <typename ValueType>
ValueType getOptional(const YAML::Node &node, const ValueType &def) {
  if (node) {
    return node.as<ValueType>();
  } else {
    return def;
  }
}

void createImageAtlas(const YAML::Node &config) {
  PROFILE(createImageAtlas);
  
  const std::string inputFolder = getOptional(config["input"], DEFAULT_INPUT);
  const std::string outputName = getOptional(config["output"], DEFAULT_OUTPUT);
  const CoordPx sep = getOptional(config["sep"], DEFAULT_SEP);
  const YAML::Node &whitepixelNode = config["whitepixel"];
  const bool recursive = getOptional(config["recursive"], false);
  const size_t maxDepth = getOptional<size_t>(
    config["max resursive depth"],
    std::numeric_limits<size_t>::max()
  );

  std::remove((outputName + ".png").c_str());
  std::vector<std::string> paths(
    recursive
    ? findFilesRec(inputFolder, extIsImage, maxDepth)
    : findFiles(inputFolder, extIsImage)
  );
  sortByFrame(paths);
  std::vector<Image> images = loadImages(paths);
  if (whitepixelNode) {
    const CoordPx size = 1 + whitepixelNode.as<CoordPx>() * 2;
    images.emplace_back(size, size, images.back().format(), 255);
  }
  std::vector<RectPx> rects = rectsFromImages(images);
  const CoordPx length = packRects(rects, sep);
  writeImage(outputName + ".png", makeAndBlit(images, rects, length));
  writeAtlas(outputName + ".atlas", paths, rects, length, whitepixelNode);
}
