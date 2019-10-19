#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

long file_size(FILE* fd)
{
	long size = 0;
	fseek(fd, 0, SEEK_END);
	size = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	
	return size;
}

short file_read_short(unsigned char** data)
{
	short res = *((short*)*data);
	*data += sizeof(short);
	return res;
}

int file_read_int(unsigned char** data)
{
	int res = *((int*)*data);
	*data += sizeof(int);
	return res;
}

void file_skip_pad(unsigned char** data, int pad)
{
	(*data) += pad;
}

void file_write_data(unsigned char** arr, unsigned char* data, int size)
{
	memcpy(*arr, data, size);
	(*arr) += size;
	/* while (size > 0)
	   {
	  	 *(*arr) = *data++;
	  	 (*arr)++;
	  	 size--;
	   }
	*/
}