#include <reader.h>
#include <stdio.h>
#include <stdlib.h>

void swapEndianness(word data[], uint32_t size) {
  for(int i = 0; i < size; ++i) {
    data[i] = (data[i] & 0xFF00) >> 8 | (data[i] & 0x00FF) << 8;
  }
}

uint32_t readFile(word** data, const char* path) {
  FILE* file = fopen(path, "rb");
  fseek(file, 0, SEEK_END);
  uint32_t size = ftell(file);
  fseek(file, 0, SEEK_SET);
  *data = (word*)malloc(size);
  uint32_t instrCount = fread(*data, sizeof(word), size/sizeof(word), file); 
  swapEndianness(*data, instrCount);
  return instrCount;
}


