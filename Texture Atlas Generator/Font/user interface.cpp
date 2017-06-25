//
//  user interface.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 3/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "user interface.hpp"

#include <set>
#include "load font.hpp"
#include "load face.hpp"
#include "write atlas.hpp"
#include "../Utils/profiler.hpp"
#include "../Utils/pack rects.hpp"
#include "../Utils/read config.hpp"
#include "../Image/blit images.hpp"
#include "../Image/write image.hpp"
#include "../Image/rects from images.hpp"
#include "../Utils/make range from vector.hpp"

static const std::string DEFAULT_OUTPUT = "output";
static const SizePx DEFAULT_SEP = 0;
static const DPI DEFAULT_DPI = {96, 96};

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

template <typename Number>
struct StepRange {
  static_assert(std::is_integral<Number>::value);
  
  Number first;
  Number last;
  Number step;
};

using PointStepRange = StepRange<SizePt>;

template <typename Number>
bool validStepRange(const StepRange<Number> range) {
  return range.first <= range.last && range.step != 0;
}

template <typename Number>
std::vector<Number> mergeStepRanges(std::vector<StepRange<Number>> &ranges) {
  std::vector<Number> nums;
  
  while (true) {
    //iterator to a range with the smallest value of first
    auto rangeOfMinFirst = ranges.end();
    for (auto r = ranges.begin(); r != ranges.end(); ++r) {
      if (r->first <= r->last && (rangeOfMinFirst == ranges.end() || r->first < rangeOfMinFirst->first)) {
        rangeOfMinFirst = r;
      }
    }
    
    if (rangeOfMinFirst == ranges.end()) {
      return nums;
    } else {
      if (nums.size() == 0 || nums.back() != rangeOfMinFirst->first) {
        nums.push_back(rangeOfMinFirst->first);
      }
      rangeOfMinFirst->first += rangeOfMinFirst->step;
    }
  }
}

std::vector<SizePt> getSizes(const YAML::Node &points) {
  if (!points.IsSequence() || points.size() == 0) {
    throw YAML::ParserException(points.Mark(), "\"points\" should be an array of ranges");
  }
  
  std::vector<PointStepRange> ranges;
  
  for (auto r = points.begin(); r != points.end(); ++r) {
    ranges.push_back({
      (*r)["first"].as<SizePt>(),
      (*r)["last"].as<SizePt>(),
      (*r)["step"].as<SizePt>()
    });
    if (!validStepRange(ranges.back())) {
      throw YAML::ParserException(r->Mark(), "Invalid range");
    }
  }
  
  return mergeStepRanges(ranges);
}

template <typename Number>
std::vector<Range<Number>> mergeRanges(std::vector<Range<Number>> &ranges) {
  static_assert(std::is_integral<Number>::value);
  
  std::sort(ranges.begin(), ranges.end(), [](const Range<Number> a, const Range<Number> b) {
    //lexicographically sort the ranges
    return a.begin() < b.begin() || a.end() < b.end();
  });
  
  std::vector<Range<Number>> result;
  Range<Number> current = ranges.front();
  for (auto r = ranges.cbegin() + 1; r != ranges.cend(); ++r) {
    if (current.end() >= r->begin()) {
      current.end(std::max(current.end(), r->end()));
    } else {
      result.push_back(current);
      current = *r;
    }
  }
  result.push_back(current);
  
  return result;
}

std::vector<CodePointRange> getChars(const YAML::Node &chars) {
  if (!chars.IsSequence() || chars.size() == 0) {
    throw YAML::ParserException(chars.Mark(), "\"chars\" should be an array of ranges");
  }
  
  std::vector<CodePointRange> ranges;
  
  for (auto r = chars.begin(); r != chars.end(); ++r) {
    ranges.push_back({
      (*r)["first"].as<CodePoint>(),
      (*r)["last"].as<CodePoint>() + 1
    });
    if (ranges.back().begin() > ranges.back().end()) {
      throw YAML::ParserException(r->Mark(), "Invalid range");
    }
  }
  
  return mergeRanges(ranges);
}

void createFontAtlas(const YAML::Node &config) {
  PROFILE(createFontAtlas);
  
  const std::string fontPath = config["font"].as<std::string>();
  const std::string outputName = getOptional(config["output"], DEFAULT_OUTPUT);
  const SizePx sep = getOptional(config["sep"], DEFAULT_SEP);
  const DPI dpi = getOptionalVector<DPI::scalar>(config["dpi"], DEFAULT_DPI);
  const std::vector<SizePt> sizes = getSizes(config["points"]);
  const std::vector<CodePointRange> chars = getChars(config["chars"]);
  
  if (sizes.size() == 0 || chars.size() == 0) {
    return;
  }
  
  const Font font = loadFont(fontPath);
  std::vector<Face> faces;
  std::vector<RectPx> greyRects;
  std::vector<RectPx> colorRects;
  for (auto s = sizes.cbegin(); s != sizes.cend(); ++s) {
    faces.push_back(loadFace(font, {dpi, *s}, chars));
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
    writeImage(outputName + "_grey.png", greyDst);
  }
  if (colorDst.s.x && colorDst.s.y) {
    writeImage(outputName + "_color.png", colorDst);
  }
  writeAtlas(outputName + ".atlasf", faces, greyRects, colorRects, greyLength, colorLength);
}

#undef CHECK_NODE
