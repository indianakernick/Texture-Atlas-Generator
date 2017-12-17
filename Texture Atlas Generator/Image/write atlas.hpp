//
//  write atlas.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 4/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef image_write_atlas_hpp
#define image_write_atlas_hpp

#include <vector>
#include "image.hpp"
#include <string_view>

class AtlasWriteError final : public std::runtime_error {
public:
  explicit AtlasWriteError(const char *);
  explicit AtlasWriteError(const std::string &);
  explicit AtlasWriteError(const std::exception &);
};

void writeAtlas(
  std::string_view,
  const std::vector<std::string> &,
  const std::vector<RectPx> &,
  CoordPx,
  bool
);

#endif
