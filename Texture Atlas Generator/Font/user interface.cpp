//
//  user interface.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 3/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "user interface.hpp"

#include "load font.hpp"
#include "load face.hpp"
#include "write atlas.hpp"
#include "../Utils/profiler.hpp"
#include "../Utils/pack rects.hpp"
#include "../Image/blit images.hpp"
#include "../Image/write image.hpp"
#include "../Image/rects from images.hpp"
#include "../Utils/make range from vector.hpp"

void createFontAtlas(
  const std::string &input,
  const std::string &output,
  const std::vector<FaceSize> &sizes,
  CodePointRange range,
  SizePx sep
) {
  PROFILE(createFontAtlas);
  
  const Font font = loadFont(input);
  std::vector<Face> faces;
  std::vector<RectPx> rects;
  for (auto s = sizes.cbegin(); s != sizes.cend(); s++) {
    faces.push_back(loadFace(font, *s, range));
    rectsFromImages(faces.back().glyphs, rects);
  }
  
  const SizePx length = packRects(rects, sep);
  Image::Format format = Image::Format::GREY;
  if (faces.size()) {
    if (faces.front().glyphs.size()) {
      format = faces.front().glyphs.front().format;
    }
  }
  Image image = makeBlitDst(length, format);
  
  Range<const RectPx *> rectsRange(nullptr, rects.data());
  for (auto f = faces.cbegin(); f != faces.cend(); f++) {
    rectsRange.begin(rectsRange.end());
    rectsRange.size(f->glyphs.size());
    if (f->glyphs.size()) {
      blitImages(
        image,
        makeRange(f->glyphs),
        rectsRange
      );
    }
  }
  
  writeImage(output + ".png", image);
  writeAtlas(output + ".atlas", faces, rects, length);
}

#undef CHECK_NODE
