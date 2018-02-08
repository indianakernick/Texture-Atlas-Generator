//
//  write atlas.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 4/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "write atlas.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include "../Utils/logger.hpp"

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

VecPx getWhitepixel(const RectPx lastRect) {
  return {
    lastRect.x + (lastRect.w - 1) / 2,
    lastRect.y + (lastRect.h - 1) / 2
  };
}

using nlohmann::json;

void to_json(json &json, const VecPx vec) {
  json = {vec.x, vec.y};
}

void to_json(json &json, const RectPx rect) {
  json = {rect.x, rect.y, rect.w, rect.h};
}

void writeAtlas(
  const std::string_view output,
  const std::vector<std::string> &paths,
  const std::vector<RectPx> &rects,
  const CoordPx size,
  const bool hasWhitepixel
) try {
  Logger::get() << "Writing atlas to file \"" << output << "\"\n";
  
  json doc;
  
  doc.emplace("size", json::array({size, size}));
  
  if (hasWhitepixel) {
    doc.emplace("whitepixel", getWhitepixel(rects.back()));
  }
  
  json &rectsNode = doc["rects"];
  rectsNode = json::array();
  const auto end = rects.cend() - hasWhitepixel;
  for (auto r = rects.cbegin(); r != end; ++r) {
    rectsNode.emplace_back(*r);
  }
  
  json &namesNode = doc["names"];
  namesNode = json::object();
  for (size_t p = 0; p != paths.size(); ++p) {
    const std::string_view name = getImageName(paths[p]);
    if (!namesNode.emplace(name, p).second) {
      throw AtlasWriteError("Two images have the same name \"" + std::string(name) + "\"");
    }
  }
  
  std::ofstream file(output.data(), std::fstream::binary);
  if (!file.is_open()) {
    throw AtlasWriteError("Could not open output file");
  }
  file << doc;
} catch (AtlasWriteError &) {
  throw;
} catch (std::exception &e) {
  throw AtlasWriteError(e);
}
