//
//  spritesheet.c
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 25/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "spritesheet.h"

#include <stdlib.h>
#include <strings.h>
#include "private spritesheet.h"

struct Spritesheet {
  RectPx *sprites;
  size_t numSprites;
  size_t numSlots;
  SizePx2 size;
  PosPx2 whitepixel;
};

size_t calcNumSlots(const size_t numSprites) {
  return numSprites * 16;
}

Spritesheet *makeSpritesheet(
  const size_t numSprites,
  const SizePx2 size,
  const PosPx2 whitepixel
) {
  Spritesheet *sheet = malloc(sizeof(Spritesheet));
  if (sheet == NULL) {
    return NULL;
  }
  
  sheet->sprites = malloc(numSprites * sizeof(RectPx));
  if (sheet->sprites == NULL) {
    free(sheet);
    return NULL;
  }
  memset(sheet->sprites, 255, numSprites * sizeof(RectPx));
  
  sheet->numSprites = numSprites;
  sheet->numSlots = calcNumSlots(numSprites);
  sheet->size = size;
  sheet->whitepixel = whitepixel;
  
  return sheet;
}

void destroySpritesheet(Spritesheet *sheet) {
  free(sheet->sprites);
  free(sheet);
}

size_t hash(const char *str) {
  size_t hash = 5381;
  char c;
  while ((c = *(str++))) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash;
}

int rectEqual(const RectPx a, const RectPx b) {
  return a.p.x == b.p.x &&
         a.p.y == b.p.y &&
         a.s.x == b.s.x &&
         a.s.y == b.s.y;
}

int insertSprite(Spritesheet *sheet, const char *name, const RectPx sprite) {
  if (sheet == NULL || sheet->sprites == NULL || name == NULL) {
    return 0;
  } else {
    const size_t index = hash(name) % sheet->numSlots;
    RectPx *const slot = sheet->sprites + index;
    if (rectEqual(*slot, NO_SPRITE)) {
      *slot = sprite;
      return 1;
    } else {
      return 0;
    }
  }
}

RectPx getSprite(const Spritesheet *sheet, const char *name) {
  if (sheet == NULL || sheet->sprites == NULL || name == NULL) {
    return NO_SPRITE;
  } else {
    return sheet->sprites[hash(name) % sheet->numSlots];
  }
}

SizePx2 getSize(const Spritesheet *sheet) {
  if (sheet == NULL) {
    return NO_SIZE;
  } else {
    return sheet->size;
  }
}

PosPx2 getWhitepixel(const Spritesheet *sheet) {
  if (sheet == NULL) {
    return NO_WHITEPIXEL;
  } else {
    return sheet->whitepixel;
  }
}

int posEqual(const PosPx2 a, const PosPx2 b) {
  return a.x == b.x &&
         a.y == b.y;
}

int hasWhitepixel(const Spritesheet *sheet) {
  if (sheet == NULL || posEqual(sheet->whitepixel, NO_WHITEPIXEL)) {
    return 0;
  } else {
    return 1;
  }
}
