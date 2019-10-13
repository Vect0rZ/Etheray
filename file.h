#ifndef FILE_H
#define FILE_H
#include <stdio.h>

long get_file_size(FILE* fd);
short get_short(unsigned char** data);
int get_int(unsigned char** data);
void skip_pad(unsigned char** data, int pad);
void set_data(unsigned char** arr, unsigned char* data, int size);

#endif