//
//  rects from images.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 11/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef rects_from_images_hpp
#define rects_from_images_hpp

#include <vector>
#include "types.hpp"
#include <Surface/surface.hpp>

std::vector<RectPx> rectsFromImages(const std::vector<Surface> &);
void rectsFromImages(const std::vector<Surface> &, std::vector<RectPx> &);

#endif
