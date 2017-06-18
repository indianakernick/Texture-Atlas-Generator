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
#include "image.hpp"

std::vector<RectPx> rectsFromImages(const std::vector<Image> &);
void rectsFromImages(const std::vector<Image> &, std::vector<RectPx> &);

#endif
