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

template <std::vector<Image> Face::*GLYPHS>
void blitGlyphs(const std::vector<Face> &faces, Image &dst, const std::vector<RectPx> &rects) {
  if (dst.s.x == 0 || dst.s.y == 0) {
    return;
  }
  
  Range<const RectPx *> rectsRange(nullptr, rects.data());
  for (auto f = faces.cbegin(); f != faces.cend(); ++f) {
    rectsRange.begin(rectsRange.end());
    rectsRange.size(((*f).*GLYPHS).size());
    blitImages(dst, makeRange(((*f).*GLYPHS)), rectsRange);
  }
}

void createFontAtlas(
  const std::string &input,
  const std::string &output,
  const std::vector<FaceSize> &sizes,
  const CodePointRange range,
  const SizePx sep
) {
  PROFILE(createFontAtlas);
  
  if (sizes.size() == 0 || range.size() == 0) {
    return;
  }
  
  const Font font = loadFont(input);
  std::vector<Face> faces;
  std::vector<RectPx> greyRects;
  std::vector<RectPx> colorRects;
  for (auto s = sizes.cbegin(); s != sizes.cend(); ++s) {
    faces.push_back(loadFace(font, *s, range));
    rectsFromImages(faces.back().greyGlyphs, greyRects);
    rectsFromImages(faces.back().colorGlyphs, colorRects);
  }
  
  const SizePx greyLength = packRects(greyRects, sep);
  Image greyDst = makeBlitDst(greyLength, Image::Format::GREY);
  
  const SizePx colorLength = packRects(colorRects, sep);
  Image colorDst = makeBlitDst(colorLength, Image::Format::RGB_ALPHA);
  
  blitGlyphs<&Face::greyGlyphs>(faces, greyDst, greyRects);
  blitGlyphs<&Face::colorGlyphs>(faces, colorDst, colorRects);
  
  if (greyDst.s.x && greyDst.s.y) {
    writeImage(output + "_grey.png", greyDst);
  }
  if (colorDst.s.x && colorDst.s.y) {
    writeImage(output + "_color.png", colorDst);
  }
  writeAtlas(output + ".atlasf", faces, greyRects, colorRects, greyLength, colorLength);
}

#undef CHECK_NODE
