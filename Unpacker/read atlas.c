//
//  read atlas.c
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 25/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "read atlas.h"

#include <stdio.h>
#include <stdlib.h>
#include "private spritesheet.h"

Spritesheet *readAtlasFromFile(const char *path) {
  //exceptions
  
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    return NULL;
  }
  if (fseek(file, 0, SEEK_END) != 0) {
    fclose(file);
    return NULL;
  }
  const long fileSize = ftell(file);
  if (fileSize == -1L) {
    fclose(file);
    return NULL;
  }
  if (fseek(file, 0, SEEK_SET) != 0) {
    fclose(file);
    return NULL;
  }
  
  uint8_t *mem = malloc(fileSize);
  if (mem == NULL) {
    fclose(file);
    return NULL;
  }
  if (fread(mem, fileSize, 1, file) != 1) {
    free(mem);
    fclose(file);
    return NULL;
  }
  
  Spritesheet *sheet = readAtlasFromMemory(mem, fileSize);
  free(mem);
  return sheet;
}

Spritesheet *readAtlasFromMemory(const uint8_t *mem, const size_t size) {
  #define DEREF(TYPE) (*((const TYPE *)mem)); \
                      mem += sizeof(TYPE)
  if (size < 20) {
    return NULL;
  }
  const uint8_t *const endMem = mem + size;
  
  const SizePx2 sheetSize = DEREF(SizePx2);
  const PosPx2 whitepixel = DEREF(PosPx2);
  if (
    whitepixel.x < -1 || whitepixel.y < -1 ||
    whitepixel.x >= (PosPx)sheetSize.x || whitepixel.y >= (PosPx)sheetSize.y
  ) {
    return NULL;
  }
  SizePx numRects = DEREF(SizePx);
  if (numRects == 0 || numRects > sheetSize.x * sheetSize.y) {
    return NULL;
  }
  
  //+ 2 for the minimum size of a sprite name
  if (size < 20 + numRects * (sizeof(RectPx) + 2)) {
    return NULL;
  }
  
  Spritesheet *sheet = makeSpritesheet(numRects, sheetSize, whitepixel);
  if (sheet == NULL) {
    return NULL;
  }
  
  const uint8_t *rectsBegin = mem;
  const uint8_t *namesBegin = mem + numRects * sizeof(RectPx);
  
  while (numRects--) {
    const uint8_t *nameEnd = namesBegin;
    while (nameEnd < endMem && *nameEnd != 0) {
      nameEnd++;
    }
    if (nameEnd >= endMem) {
      destroySpritesheet(sheet);
      return NULL;
    }
    
    if (insertSprite(
      sheet,
      (const char *)namesBegin,
      *((const RectPx *)rectsBegin)
    ) == 0) {
      destroySpritesheet(sheet);
      return NULL;
    }
  }
  
  return sheet;
}
