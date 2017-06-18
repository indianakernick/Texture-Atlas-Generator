//
//  search dir.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 22/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef utils_search_dir_hpp
#define utils_search_dir_hpp

#include <vector>
#include <string>
#include <experimental/string_view>

class DirSearchError final : public std::runtime_error {
public:
  DirSearchError();
};

class NoSupportedFilesError final : public std::runtime_error {
public:
  NoSupportedFilesError();
};

using StringView = std::experimental::string_view;
using SearchPred = bool(*)(StringView);

bool extIsImage(StringView);
std::vector<std::string> findFiles(const std::string &, SearchPred);

#endif
