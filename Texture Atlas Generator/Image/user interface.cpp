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
#include "rects from images.hpp"
#include "../Utils/profiler.hpp"
#include "../Utils/pack rects.hpp"
#include "../Utils/search dir.hpp"
#include "../Utils/read config.hpp"

static const std::string DEFAULT_INPUT = ".";
static const std::string DEFAULT_OUTPUT = "output";
static const SizePx DEFAULT_SEP = 1;

void createImageAtlas(const YAML::Node &config) {
  PROFILE(createImageAtlas);
  
  const std::string inputFolder = getOptional(config["input"], DEFAULT_INPUT);
  const std::string outputName = getOptional(config["output"], DEFAULT_OUTPUT);
  const SizePx sep = getOptional(config["sep"], DEFAULT_SEP);
  const YAML::Node &whitepixelNode = config["whitepixel"];

  std::remove((outputName + ".png").c_str());
  const std::vector<std::string> paths(findFiles(inputFolder, extIsImage));
  std::vector<Image> images = loadImages(paths);
  if (whitepixelNode) {
    const SizePx size = 1 + whitepixelNode.as<SizePx>() * 2;
    images.emplace_back(size, size, images.back().format, 255);
  }
  std::vector<RectPx> rects = rectsFromImages(images);
  const SizePx length = packRects(rects, sep);
  writeImage(outputName + ".png", makeAndBlit(images, rects, length));
  writeAtlas(outputName + ".atlas", paths, rects, length, whitepixelNode);
}
