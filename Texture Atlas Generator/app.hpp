//
//  app.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 24/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef app_hpp
#define app_hpp

#include <string>
#include <vector>

class ArgError final : public std::runtime_error {
public:
  ArgError();
};

class InvalidArgVal final : public std::runtime_error {
public:
  explicit InvalidArgVal(const char *);
};

void printBriefHelp();
void runApp(const std::vector<std::string> &);

#endif
