#include <stdio.h>
#include <writer.h>

void writeProgramToFile(byte* program, uint32_t size, const char* path) {
  FILE* file = fopen(path, "wb");
  if(file == NULL) {
    printf("Could not create file %s\n", path);
    fflush(stdout);
    return;
  }
  for(uint32_t i = 0; i < size; i+=2) {
    if(i+1>size)
      fwrite(0x00, sizeof(program[0]), 1, file);  
    else
      fwrite(&program[i+1], sizeof(program[0]), 1, file);  
    fwrite(&program[i], sizeof(program[0]), 1, file);  
    //if(program[i] + (program[i+1]<<8) == 0x0000)
     // break;
  }
  fclose(file);
}
