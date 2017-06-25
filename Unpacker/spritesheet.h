//
//  spritesheet.h
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 25/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef spritesheet_h
#define spritesheet_h

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int32_t PosPx;
typedef uint32_t SizePx;

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
  PosPx2 p;
  SizePx2 s;
} RectPx;
#pragma pack()

//8 0xFF bytes
static const PosPx2 NO_WHITEPIXEL = {-1, -1};
//8 0xFF bytes
static const SizePx2 NO_SIZE = {UINT32_MAX, UINT32_MAX};
//16 0xFF bytes
static const RectPx NO_SPRITE = {{-1, -1}, {UINT32_MAX, UINT32_MAX}};

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

#ifdef __cplusplus
}
#endif

#endif
