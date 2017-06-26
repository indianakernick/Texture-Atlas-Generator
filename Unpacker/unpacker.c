//
//  unpacker.c
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 26/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "unpacker.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRINT_ERR(...) fprintf(stderr, __VA_ARGS__)

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
    //on some systems size_t is unsigned long long and sometimes it unsigned long
    PRINT_ERR("Failed to allocate %llu bytes of memory\n", (unsigned long long)sizeof(Spritesheet));
    return NULL;
  }
  
  sheet->sprites = malloc(numSprites * sizeof(RectPx));
  if (sheet->sprites == NULL) {
    PRINT_ERR("Failed to allocate %llu bytes of memory\n", (unsigned long long)sizeof(Spritesheet));
    free(sheet);
    return NULL;
  }
  memset(sheet->sprites, 0, numSprites * sizeof(RectPx));
  
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
  str--;
  while ((c = *(++str))) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash;
}

int nullRect(const RectPx a) {
  return a.x == NO_SPRITE.x &&
         a.y == NO_SPRITE.y &&
         a.w == NO_SPRITE.w &&
         a.y == NO_SPRITE.h;
}

int insertSprite(Spritesheet *sheet, const char *name, const RectPx sprite) {
  if (sheet == NULL || sheet->sprites == NULL || name == NULL) {
    return 0;
  } else {
    const size_t index = hash(name) % sheet->numSlots;
    RectPx *const slot = sheet->sprites + index;
    if (nullRect(*slot)) {
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

int nullPos(const PosPx2 a) {
  return a.x == NO_WHITEPIXEL.x &&
         a.y == NO_WHITEPIXEL.y;
}

int hasWhitepixel(const Spritesheet *sheet) {
  if (sheet == NULL || nullPos(sheet->whitepixel)) {
    return 0;
  } else {
    return 1;
  }
}

Spritesheet *readAtlasFromFile(const char *path) {
  //exceptions
  PRINT_ERR("Opening file \"%s\"\n", path);
  
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    PRINT_ERR("Failed to open file\n");
    return NULL;
  }
  fseek(file, 0, SEEK_END);
  const long fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);
  
  if (fileSize < 1) {
    PRINT_ERR("Failed to get the size of the file\n");
    fclose(file);
    return NULL;
  }
  
  uint8_t *mem = malloc(fileSize);
  if (mem == NULL) {
    PRINT_ERR("Failed to allocate %li bytes of memory\n", fileSize);
    fclose(file);
    return NULL;
  }
  if (fread(mem, fileSize, 1, file) != 1) {
    PRINT_ERR("Failed to copy file into memory\n");
    free(mem);
    fclose(file);
    return NULL;
  }
  
  Spritesheet *sheet = readAtlasFromMemory(mem, fileSize);
  free(mem);
  fclose(file);
  return sheet;
}

Spritesheet *readAtlasFromMemory(const uint8_t *mem, const size_t size) {
  #define DEREF(TYPE) (*((const TYPE *)mem)); \
                      mem += sizeof(TYPE)
  
  PRINT_ERR("Reading atlas from memory\n");
  
  if (size < 20) {
    PRINT_ERR("Currupt atlas");
    return NULL;
  }
  const uint8_t *const endMem = mem + size;
  
  const SizePx2 sheetSize = DEREF(SizePx2);
  if (sheetSize.x < 1 || sheetSize.y < 1) {
    PRINT_ERR("Negative sheet size\n");
    return NULL;
  }
  
  const PosPx2 whitepixel = DEREF(PosPx2);
  if (
    whitepixel.x < -1 || whitepixel.y < -1 ||
    whitepixel.x >= (PosPx)sheetSize.x || whitepixel.y >= (PosPx)sheetSize.y
  ) {
    PRINT_ERR("Whitepixel position out of range\n");
    return NULL;
  }
  
  SizePx numRects = DEREF(SizePx);
  if (numRects < 0 || numRects > sheetSize.x * sheetSize.y) {
    PRINT_ERR("Number of rectangles out of range\n");
    return NULL;
  }
  
  #undef DEREF
  
  //+ 2 for the minimum size of a sprite name
  if (size < 20 + numRects * (sizeof(RectPx) + 2)) {
    PRINT_ERR("Currupt atlas\n");
    return NULL;
  }
  
  Spritesheet *sheet = makeSpritesheet(numRects, sheetSize, whitepixel);
  if (sheet == NULL) {
    PRINT_ERR("Failed to initialize Spritesheet object\n");
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
      PRINT_ERR("Currupt strings\n");
      destroySpritesheet(sheet);
      return NULL;
    }
    
    if (insertSprite(
      sheet,
      (const char *)namesBegin,
      *((const RectPx *)rectsBegin)
    ) == 0) {
      PRINT_ERR("Hash table collision\n");
      destroySpritesheet(sheet);
      return NULL;
    }
    rectsBegin += sizeof(RectPx);
    namesBegin = nameEnd + 1;
  }
  
  return sheet;
}

#undef PRINT_ERR
