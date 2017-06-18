//
//  write atlas.cpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 4/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "write atlas.hpp"

#include <fstream>
#include <iostream>
#include "../Utils/range.hpp"
#include "../Utils/profiler.hpp"
#include "../Utils/write atlas.hpp"

void writeFontMetrics(YAML::Emitter &emitter, const FaceMetrics &metrics) {
  emitter <<
  YAML::BeginMap <<
    YAML::Key << "line height" << YAML::Value << metrics.lineHeight <<
    YAML::Key << "min y" << YAML::Value << metrics.minY <<
    YAML::Key << "max y" << YAML::Value << metrics.maxY <<
  YAML::EndMap;
}

void writeMetrics(YAML::Emitter &emitter, const std::vector<GlyphMetrics> &metrics) {
  emitter << YAML::BeginSeq;
  for (auto m = metrics.cbegin(); m != metrics.cend(); m++) {
    emitter <<
    YAML::BeginMap <<
      YAML::Key << "bearing" << YAML::Value <<
        YAML::Flow << YAML::BeginSeq << m->bearing.x << m->bearing.y << YAML::EndSeq <<
      YAML::Key << "size" << YAML::Value <<
        YAML::Flow << YAML::BeginSeq << m->size.x << m->size.y << YAML::EndSeq <<
      YAML::Key << "advance" << YAML::Value << m->advance <<
    YAML::EndMap;
  }
  emitter << YAML::EndSeq;
}

void writeGlyphs(YAML::Emitter &emitter, Range<const RectPx *> rects) {
  emitter << YAML::BeginSeq;
  for (auto r = rects.cbegin(); r != rects.cend(); r++) {
    if (r->s.x == 0 || r->s.y == 0) {
      emitter << YAML::Flow << YAML::BeginSeq << 0 << 0 << 0 << 0 << YAML::EndSeq;
    } else {
      emitter << YAML::Flow << YAML::BeginSeq <<
        r->p.x <<
        r->p.y <<
        (r->p.x + r->s.x - 1) <<
        (r->p.y + r->s.y - 1) <<
      YAML::EndSeq;
    }
  }
  emitter << YAML::EndSeq;
}

void writeFaces(YAML::Emitter &emitter, const std::vector<Face> &faces, const std::vector<RectPx> &rects) {
  Range<const RectPx *> rectsRange(nullptr, rects.data());
  
  emitter << YAML::BeginSeq;
  for (auto f = faces.cbegin(); f != faces.cend(); f++) {
    rectsRange.begin(rectsRange.end());
    rectsRange.size(f->glyphs.size());
    
    emitter <<
      YAML::BeginMap <<
        YAML::Key << "range" << YAML::Value << YAML::Flow << YAML::BeginSeq <<
          static_cast<int64_t>(f->range.begin()) << static_cast<int64_t>(f->range.end()) <<
        YAML::EndSeq <<
        YAML::Key << "points" << YAML::Value << f->size.points <<
        YAML::Key << "font metrics" << YAML::Value;
    
    writeFontMetrics(emitter, f->faceMetrics);
    
    emitter <<
      YAML::Key << "glyph metrics" << YAML::Value;
    
    writeMetrics(emitter, f->glyphMetrics);
    
    emitter <<
      YAML::Key << "glyphs" << YAML::Value;
    
    writeGlyphs(emitter, rectsRange);
    
    if (f->kerning.size()) {
      emitter <<
        YAML::Key << "has kerning" << YAML::Value << true <<
        YAML::Key << "kerning" << YAML::Value << f->kerning;
    } else {
      emitter <<
        YAML::Key << "has kerning" << YAML::Value << false;
    }
    
    emitter << YAML::EndMap;
  }
  emitter << YAML::EndSeq;
}

void writeAtlas(
  const std::string &output,
  const std::vector<Face> &faces,
  const std::vector<RectPx> &rects,
  SizePx texSize
) {
  PROFILE(writeAtlas(Font));
  
  std::cout << "Writing atlas to file \"" << output << "\"\n";
  
  std::ofstream file(output);
  YAML::Emitter emitter(file);
  
  checkEmitter(emitter);
  
  emitter << YAML::BeginDoc << YAML::BeginMap <<
    YAML::Key << "type" << YAML::Value << "font" <<
    YAML::Key << "size" << YAML::Value << YAML::Flow << YAML::BeginSeq <<
      texSize << texSize <<
    YAML::EndSeq <<
    YAML::Key << "faces" << YAML::Value;
  
  writeFaces(emitter, faces, rects);
  
  emitter << YAML::EndMap << YAML::EndDoc;
  
  checkEmitter(emitter);
}
