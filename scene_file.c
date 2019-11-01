#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "scene_file.h"

Scene scene_file_read(char* file)
{
    printf("a");
    Scene scene;

    FILE* fd = fopen(file, "r+");
    long line_size = 128;

    char* data = malloc(sizeof(char) * line_size);
    char* line = fgets(data, line_size, fd);
    printf("b");
    while (line != NULL)
    {
        printf("%s", data);
        if (data[0] == 'W')
        {
            printf("c");
            Tokens res = tokenize(data, ' ');
            printf("Token1: %s\n", res.tokens[0]);
            printf("Token2: %s\n", res.tokens[1]);
        }
            
        line = fgets(data, line_size, fd);
        printf("d");
    }

    free(data);
    printf("e");

    return scene;
}

Tokens tokenize(char* str, char delimiter)
{
    char* strptr = str;
    int tokens = 1;
    int i = 0, j = 0;

    while (*strptr++ != '\0')
        if (*strptr == delimiter)
            tokens++;
    printf("Tokens: %i\n", tokens);

    Tokens res;
    res.count = tokens;
    res.tokens = malloc(sizeof(char*) * tokens);

    /* Point back to begining of the string */
    strptr = str;
    printf("1");
    for (i = 0; i < tokens; i++)
    {
        printf("2");
        /* Allocate max 16 chars for token */
        res.tokens[i] = malloc(sizeof(char) * 16);
        printf("3");
        do {
            res.tokens[i][j++] = *strptr;
            strptr++;
        } while (*strptr != delimiter);
        printf("5");
        
        j = 0;
    }

    return res;
}