//
//  blit images.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 23/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef image_blit_images_hpp
#define image_blit_images_hpp

#include <vector>
#include "types.hpp"
#include <Surface/surface.hpp>

Surface makeBlitDst(CoordPx, Surface::BytesPerPixel);
void blitImages(Surface &, const std::vector<Surface> &, const std::vector<RectPx> &);
Surface makeAndBlit(const std::vector<Surface> &, const std::vector<RectPx> &, CoordPx);

#endif
