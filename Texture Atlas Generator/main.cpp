//
//  main.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "app.hpp"
#include <iostream>
#include "profiler.hpp"

int main(int argc, const char * argv[]) {
  const std::vector<std::string> args(argv, argv + argc);
  #if RELEASE
  try {
    runApp(args);
  } catch (ArgError &e) {
    printBriefHelp();
    return EXIT_FAILURE;
  } catch (std::exception &e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }
  #else
  runApp(args);
  #endif
  
  PROFILER_INFO(stdout);
  
  return EXIT_SUCCESS;
}
