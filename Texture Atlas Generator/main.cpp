//
//  main.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "app.hpp"
#include <iostream>

int main(int argc, const char *argv[]) {
  const std::vector<std::string> args(argv, argv + argc);
  #ifdef RELEASE
  try {
    runApp(args);
  } catch (ArgError &e) {
    printUsage();
    return EXIT_SUCCESS;
  } catch (std::exception &e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }
  #else
  //In debug builds, the debugger will pause at the throwing of an uncaught exception
  runApp(args);
  #endif
  
  return EXIT_SUCCESS;
}
