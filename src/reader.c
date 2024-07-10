#include <reader.h>
#include <stdio.h>
#include <stdlib.h>
uint32_t readFile(word** data, const char* path) {
  FILE* file = fopen(path, "rb");
  fseek(file, 0, SEEK_END);
  uint32_t size = ftell(file);
  fseek(file, 0, SEEK_SET);
  *data = (word*)malloc(size);
  uint32_t instrCount = fread(*data, sizeof(word), size/sizeof(word), file); 
  return instrCount;
}
