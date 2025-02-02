#include <reader.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


uint32_t readFile(byte** data, const char* path) {
  FILE* file = fopen(path, "rb");
  if(file == NULL) {
    printf("Could not find file %s\n", path);
    fflush(stdout);
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  uint32_t size = ftell(file);
  fseek(file, 0, SEEK_SET);
  *data = (byte*)malloc(size);
  memset(*data, 0, size);
  uint32_t instrCount = fread(*data, 1, size, file); 
  fclose(file);
  return instrCount;
}


