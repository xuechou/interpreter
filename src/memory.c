#include <stdlib.h>

#include "memory.h"

//TODO: 为什么不直接使用realloc,即使newSize=0也可以使用realloc啊？ oldSize没有用到
void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
  if (newSize == 0) {
    free(pointer);
    return NULL;
  }

  void* result = realloc(pointer, newSize);
  if (result == NULL) exit(1);
  return result;
}