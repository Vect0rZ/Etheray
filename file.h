#ifndef FILE_H
#define FILE_H
#include <stdio.h>

long file_size(FILE* fd);
short file_read_short(unsigned char** data);
int file_read_int(unsigned char** data);
void file_skip_pad(unsigned char** data, int pad);
void file_write_data(unsigned char** arr, unsigned char* data, int size);

#endif