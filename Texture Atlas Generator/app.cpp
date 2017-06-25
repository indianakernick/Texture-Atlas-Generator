//
//  app.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 24/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "app.hpp"

#include <iostream>
#include "Font/user interface.hpp"
#include "Image/user interface.hpp"

static constexpr char USAGE[] = R"(Usage:
  atlasgen <path_to_config_file>
)";

ArgError::ArgError()
  : std::runtime_error("Invalid arguments") {}

void printUsage() {
  std::cout << USAGE;
}

void runApp(const std::vector<std::string> &args) {
  if (args.size() < 2) {
    throw ArgError();
  }
  
  std::string pathToConfig = args[1];
  for (auto a = args.cbegin() + 2; a != args.cend(); ++a) {
    pathToConfig += ' ';
    pathToConfig += *a;
  }
  
  const YAML::Node config = YAML::LoadFile(pathToConfig);
  const std::string &jobType = config["job type"].as<std::string>();
  if (jobType == "font") {
    createFontAtlas(config);
  } else if (jobType == "spritesheet") {
    createImageAtlas(config);
  } else {
    throw std::runtime_error("\"job type\" is an invalid value. Should be \"font\" or \"spritesheet\"");
  }
}
