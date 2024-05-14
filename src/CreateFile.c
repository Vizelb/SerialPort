#include "../inc/CreateFile.h"
#include "../inc/WorkWithFile.h"


FILE *file_ch;
extern FILE *file;

BOOL InitCreateFile()
{
    char fname[MAX_PATH];

    printf("\nWrite file name: ");
    scanf("%s", fname);

    printf("\nFile - %s\n", fname);

    file = fopen(fname, "rb");

    if (!file) {
        perror("\nFile opening failed\n");
        return FALSE;
    }

    printf("\nFile opened!\n");
    return TRUE;
}

BOOL CF_InitCreateFile()
{
    int i, j;

    //char fname[MAX_PATH];
    char fname[MAX_PATH];
    printf("\nWrite file name: ");
    //fname = "word";
    //scanf("%[^\n]s", fname);
    scanf("%s", fname);


    printf("\nFile - %s\n", fname);

    file_ch = fopen(fname, "rb");

    if (!file_ch) {
        perror("\nFile opening failed\n");
        return FALSE;
    }
//sleep(10);
    /*for (i = 0, j = 0x5; i < 77655; i++)  // запись в файл
        fprintf(file_ch, "%x", j);*/

    printf("\nFile opened!\n");

    //fclose(file_ch);
    return TRUE;
}
