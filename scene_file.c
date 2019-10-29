#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "scene_file.h"

Object* scene_file_read(char* file)
{
    FILE* fd = fopen(file, "r+");
    long fsize = file_size(fd);

    char* data = malloc(sizeof(char) * fsize);
    fread(data, sizeof(char), fsize, fd);

    char* line = malloc(sizeof(char) * 128);
    char* p = data;
    do
    {
        int width;
        int height;
        int l = 0;
        while(*p != '\n')
        {
            line[l++] = *p++;
        }
        line[l+1] = '\0';

        printf("{%s}\n", line);
    } while (*p++ != EOF);

    free(data);
    free(line);

    return 0;
}