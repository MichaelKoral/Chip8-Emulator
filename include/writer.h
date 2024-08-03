#ifndef WRITER_H
#define WRITER_H
#include <chip8.h>
void writeProgramToFile(byte* program, uint32_t size, const char* path);
#endif
