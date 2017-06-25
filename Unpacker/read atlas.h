//
//  read atlas.h
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 25/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef read_atlas_h
#define read_atlas_h

#include "spritesheet.h"

#ifdef __cplusplus
extern "C" {
#endif

Spritesheet *readAtlasFromFile(const char *);
Spritesheet *readAtlasFromMemory(const uint8_t *, size_t);

#ifdef __cplusplus
}
#endif

#endif
