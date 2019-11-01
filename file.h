#ifndef FILE_H
#define FILE_H
#include <stdio.h>

/**
 * Returns the size of the file in bytes.
 * */
long file_size(FILE* fd);
/**
 * Returns the next short and increments the data pointer by sizeof(short).
 * */
short file_read_short(unsigned char** data);
/**
 * Returns the next integer and increments the data pointer by sizeof(int).
 * */
int file_read_int(unsigned char** data);
/**
 * Increments the data pointer by [pad] bytes.
 * */
void file_skip_pad(unsigned char** data, int pad);
/**
 * Copy [size] amount of data into arr.
 * */
void file_write_data(unsigned char** arr, unsigned char* data, int size);

#endif
