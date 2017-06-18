//
//  load face.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 24/4/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef load_face_hpp
#define load_face_hpp

#include "font.hpp"
#include "face.hpp"

class GlyphLoadError final : public std::runtime_error {
public:
  GlyphLoadError(CodePoint, const char *);
};

Face loadFace(const Font &, const FaceSize &, CodePointRange);

#endif
