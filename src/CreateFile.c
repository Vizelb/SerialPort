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

    char fname[MAX_PATH];
    printf("file name: ");
    scanf("%[^\n]s", fname);

    file = fopen(fname, "w+t");

    if (!file) {
        perror("File opening failed");
    }

    /*for (i = 0, j = 0x5; i < 77655; i++)  // запись в файл
        fprintf(file, "%x", j);*/

    printf("\nFile created!");

    fclose(file);
}
