#include "../inc/CreateFile.h"

#include <stdio.h>
#include <stdlib.h>

#include <inttypes.h>

#include <string.h>
#include <windows.h>



FILE *file;


void InitCreateFile()
{
    int i, j;

    file = fopen(newFile, "w+t");

    for (i = 0, j = 0x5; i < 77655; i++)
        fprintf(file, "%x", j);

    printf("\nFile created!");

    fclose(file);
}
