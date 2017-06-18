//
//  write atlas.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 23/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "write atlas.hpp"

AtlasWriteError::AtlasWriteError(const std::string &error)
  : std::runtime_error("Failed to write atlas to file: " + error) {}

void checkEmitter(const YAML::Emitter &emitter) {
  if (!emitter.good()) {
    throw AtlasWriteError(emitter.GetLastError());
  }
}
