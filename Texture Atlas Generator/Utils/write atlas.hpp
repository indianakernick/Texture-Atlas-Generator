//
//  write atlas.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 23/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef utils_write_atlas_hpp
#define utils_write_atlas_hpp

#include <yaml-cpp/yaml.h>

class AtlasWriteError final : public std::runtime_error {
public:
  explicit AtlasWriteError(const std::string &);
};

void checkEmitter(const YAML::Emitter &);

#endif
