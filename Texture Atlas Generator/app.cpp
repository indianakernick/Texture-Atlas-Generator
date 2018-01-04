//
//  app.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 24/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "app.hpp"

#include <fstream>
#include "Utils/logger.hpp"
#include "Image/user interface.hpp"

using nlohmann::json;

constexpr char USAGE[] = R"(Usage:
  packer [--silent] <path_to_config_file>

Options:
  --silent      Suppress status messages but not error messages
)";

constexpr size_t NUM_OPTIONS = 1;

ArgError::ArgError()
  : std::runtime_error("Invalid arguments") {}

void printUsage() {
  std::cout << USAGE;
}

void runApp(const std::vector<std::string> &args) {
  if (args.size() < 2) {
    throw ArgError();
  }
  
  LogOutput output = LogOutput::COUT;
  size_t nextIndex = 1;
  size_t options = NUM_OPTIONS;
  
  while (options-- && args[nextIndex][0] == '-') {
    if (args[nextIndex] == "--silent") {
      output = LogOutput::SILENT;
    }
    nextIndex++;
  }
  
  Logger::init(output);
  std::fstream file(args[nextIndex]);
  json config;
  file >> config;
  createImageAtlas(config);
}
