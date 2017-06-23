#ifdef DEBUG

#include <assert.h>
void *checkAllocSize(const size_t size, void *const context) {
  //Keep this in sync with resize image.hpp
  assert(size < (32 * 1024 * 1024));
  return context;
}
#define STBIR_MALLOC(size, context) checkAllocSize(size, context)

#else

#define STBIR_MALLOC(size, context) (context)

#endif

#define STBIR_FREE(ptr, context)
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
