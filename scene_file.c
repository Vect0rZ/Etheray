#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "scene_file.h"
#include "enums.h"
#include "sphere.h"
#include "triangle.h"
#include "plane.h"

Scene scene_file_read(char* file)
{
    printf("a");
    Scene scene;

    FILE* fd = fopen(file, "r+");
    long line_size = 128;

    char* data = malloc(sizeof(char) * line_size);
    char* line = fgets(data, line_size, fd);
    Object current;
    while (line != NULL)
    {
        printf("%s", data);
        Tokens res = tokenize(data, ' ');
        char* id = res.tokens[0];
        if (strcmp(id, "W") == 0) {
            scene.width = atoi(res.tokens[1]);
        }
        if (strcmp(id, "H") == 0) {
            scene.height = atoi(res.tokens[1]);
        }
        if (strcmp(id, "NO") == 0) {
            Tokens res = tokenize(data, ' ');

            int n = atoi(res.tokens[1]);
            scene.n_objects = n;
            scene.objects = malloc(sizeof(scene.objects) * n);
        }
        if (strcmp(id, "Object [") == 0) {
            
        }
        if (strcmp(id, "Type")) {
            if (strcmp(res.tokens[1], "Sphere") == 0) {
                current.type = SPHERE;
            }
            else if (strcmp(res.tokens[1], "Plane") == 0) {
                current.type = PLANE;
            }
            else if (strcmp(res.tokens[1], "Triangle") == 0) {
                current.type = TRIANGLE;
            }
        }
        if (strcmp(id, "Origin") == 0) {
            
        }

        line = fgets(data, line_size, fd);
    }

    free(data);
    printf("e\n");

    return scene;
}

Tokens tokenize(char* str, char delimiter)
{
    char* strptr = str;
    int tokens = 1;
    int i = 0, j = 0;

    /* Skip leading chars */
    while (*strptr == delimiter)
        strptr++;

    while (*strptr++ != '\0')
        if (*strptr == delimiter)
            tokens++;
        
    printf("Tokens: %i\n", tokens);

    Tokens res;
    res.tokens = malloc(sizeof(res.tokens) * tokens);
    res.count = tokens;

    /* Point back to beginixng of the string */
    strptr = str;
    for (i = 0; i < tokens; i++)
    {
        /* Allocate max 16 chars for token */
        res.tokens[i] = malloc(sizeof(char) * 16);
        while (*strptr != delimiter && *strptr != '\0') {
            res.tokens[i][j++] = *strptr;
            strptr++;
        };

        /* Skip the delimiter char */
        strptr++;
        j = 0;
    }

    return res;
}