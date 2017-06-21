//
//  load face.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 24/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "load face.hpp"

#include "freetype error.hpp"
#include FT_GLYPH_H
#include <cmath>
#include <iostream>
#include "../Utils/math.hpp"
#include "../Utils/profiler.hpp"
#include "../Image/blit images.hpp"

GlyphLoadError::GlyphLoadError(CodePoint c, const char *what)
  : std::runtime_error("Error loading glyph " + std::to_string(c) + ": " + what) {}

std::vector<PosPx> getKerning(const Font &face, const CodePointRange range) {
  PROFILE(getKerning);
  
  if (!FT_HAS_KERNING(face)) {
    std::cout << "Font doesn't support kerning\n";
    return {};
  }
  std::cout << "Reading kerning data\n";
  
  std::vector<PosPx> kerning;
  const CodePoint numChars = range.distance();
  kerning.reserve(numChars * numChars);
  
  for (CodePoint l = range.begin(); l != range.end(); l++) {
    for (CodePoint r = range.begin(); r != range.end(); r++) {
      FT_Vector kernVec;
      FT_Get_Kerning(
        face,
        FT_Get_Char_Index(face, l),
        FT_Get_Char_Index(face, r),
        FT_KERNING_DEFAULT,
        &kernVec
      );
      kerning.push_back(static_cast<PosPx>(divRound(kernVec.x, 64)));
    }
  }
  
  return kerning;
}

GlyphMetrics getGlyphMetrics(const FT_Glyph_Metrics &metrics) {
  return {
    //bearing
    {
      divRound(metrics.horiBearingX, 64),
      divRound(metrics.horiBearingY, 64)
    },
    //size
    {
      divRound(metrics.width, 64),
      divRound(metrics.height, 64)
    },
    //advance
    static_cast<PosPx>(divRound(metrics.horiAdvance, 64))
  };
}

FaceMetrics getFontMetrics(const Font &face) {
  return {
    //lineHeight
    static_cast<SizePx>(
      divRound(
        face->size->metrics.height,
        64
      )
    ),
    //minY
    static_cast<PosPx>(
      divRound(
        FT_MulFix(face->bbox.yMin, face->size->metrics.y_scale),
        64
      )
    ),
    //maxY
    static_cast<PosPx>(
      divRound(
        FT_MulFix(face->bbox.yMax, face->size->metrics.y_scale),
        64
      )
    )
  };
}

class BitmapConvertError {
public:
  explicit BitmapConvertError(const char *what)
    : what(what) {}
    
  const char *what;
};

uint32_t BGRAtoRGBA(const uint32_t pixel) {
  float r = ((pixel >> 8) & 255) / 255.0f;
  float g = ((pixel >> 16) & 255) / 255.0f;
  float b = (pixel >> 24) / 255.0f;
  const float a = (pixel & 255) / 255.0f;
  r /= a;
  g /= a;
  b /= a;
  r = std::pow(r, 2.2f);
  g = std::pow(g, 2.2f);
  b = std::pow(b, 2.2f);
  uint32_t out = 0;
  out |= static_cast<uint32_t>(r * 255.0f) << 24;
  out |= static_cast<uint32_t>(g * 255.0f) << 16;
  out |= static_cast<uint32_t>(b * 255.0f) << 8;
  out |= pixel & 255;
  return out;
}

Image convertBitmap(FT_Bitmap &bitmap) {
  PROFILE(convertBitmap);
  if (bitmap.pixel_mode == FT_PIXEL_MODE_GRAY) {
    if (static_cast<int>(bitmap.width) == bitmap.pitch) {
      Image image(
        bitmap.width,
        bitmap.rows,
        Image::Format::GREY,
        bitmap.buffer,
        std::free
      );
      //Image takes ownership of the FT_Bitmap
      bitmap.buffer = nullptr;
      
      return image;
    } else {
      PROFILE(copy);
      
      Image image(bitmap.width, bitmap.rows, Image::Format::GREY);
      const Image glyphImage(bitmap.width, bitmap.rows, Image::Format::GREY, bitmap.buffer, noDelete);
      
      blit(image, glyphImage);
      
      return image;
    }
  } else if (bitmap.pixel_mode == FT_PIXEL_MODE_BGRA) {
    PROFILE(Color);
    
    Image image(bitmap.width, bitmap.rows, Image::Format::RGB_ALPHA);
    convert(image, bitmap.pitch, bitmap.width, bitmap.rows, bitmap.buffer, BGRAtoRGBA);
    return image;
  } else {
    throw BitmapConvertError("Glyph is in unsupported format");
  }
}

Face loadFace(const Font &font, const FaceSize &size, const CodePointRange range) {
  PROFILE(loadFace);
  
  std::cout << "Rendering glyphs\n";

  std::vector<GlyphMetrics> metrics;
  std::vector<Image> images;
  metrics.reserve(range.size());
  images.reserve(range.size());
  
  CHECK_FT_ERROR(FT_Set_Char_Size(font, 0, size.points * 64, size.dpi.x, size.dpi.y));
  
  for (CodePoint c = range.begin(); c != range.end(); c++) {
    CHECK_FT_ERROR(FT_Load_Char(font, c, FT_LOAD_RENDER));

    if (font->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
      throw GlyphLoadError(c, "Glyph is in unsupported format");
    }

    metrics.push_back(getGlyphMetrics(font->glyph->metrics));
    try {
      images.push_back(convertBitmap(font->glyph->bitmap));
    } catch (BitmapConvertError &e) {
      throw GlyphLoadError(c, e.what);
    }
  }
  
  return {
    getKerning(font, range),
    std::move(metrics),
    std::move(images),
    getFontMetrics(font),
    size,
    range
  };
}
