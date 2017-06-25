//
//  private spritesheet.h
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 25/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef private_spritesheet_h
#define private_spritesheet_h

//private interface to spritesheet

#include "spritesheet.h"

Spritesheet *makeSpritesheet(size_t, SizePx2, PosPx2);

///Returns 0 on collision
int insertSprite(Spritesheet *, const char *, RectPx);

#endif
