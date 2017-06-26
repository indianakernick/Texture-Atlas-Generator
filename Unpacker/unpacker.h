//
//  unpacker.h
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 26/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef unpacker_h
#define unpacker_h

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)

typedef int32_t PosPx;
typedef int32_t SizePx;

#pragma pack(1)
typedef struct {
  PosPx x;
  PosPx y;
} PosPx2;
#pragma pack()

#pragma pack(1)
typedef struct {
  SizePx x;
  SizePx y;
} SizePx2;
#pragma pack()

#pragma pack(1)
typedef struct {
  PosPx x;
  PosPx y;
  SizePx w;
  SizePx h;
} RectPx;
#pragma pack()

//8 0xFF bytes
static const PosPx2 NO_WHITEPIXEL = {-1, -1};
//8 0x00 bytes
static const SizePx2 NO_SIZE = {0, 0};
//16 0x00 bytes
static const RectPx NO_SPRITE = {0, 0, 0, 0};

typedef struct Spritesheet Spritesheet;

///Must be called on the spritesheet to free it
void destroySpritesheet(Spritesheet *);

///Returns NO_SPRITE if a sprite with that name doesn't exist
RectPx getSprite(const Spritesheet *, const char *);

///Returns NO_SIZE if the spritesheet is NULL
SizePx2 getSize(const Spritesheet *);

///Returns NO_WHITEPIXEL if there isn't a whitepixel
PosPx2 getWhitepixel(const Spritesheet *);

///Returns 1 if there is a whitepixel
int hasWhitepixel(const Spritesheet *);

Spritesheet *readAtlasFromFile(const char *);
Spritesheet *readAtlasFromMemory(const uint8_t *, size_t);

#pragma GCC visibility pop

#ifdef __cplusplus
}
#endif

#endif
