#ifndef SCENE_FILE_H
#define SCENE_FILE_H

#include "object.h"

typedef struct scene {
    int width;
    int height;
    Object objects[];
} Scene;

typedef struct tokens
{
    int count;
    char** tokens;
} Tokens;

Scene scene_file_read(char* file);
Tokens tokenize(char* str, char delimiter);

#endif