//#include <../inc/WorkWithFile.h>
#include "../inc/WorkWithFile.h"

#include <stdio.h>
#include <stdlib.h>


// Description:
// ������ ��� ������ �� ����� ����� ��������� �� ������� "size + 1"


FILE *file;
char name[] = "test1.txt";
char buffer[128];

//fopen(D:\Danya\Libary\C\test1, "r");  // ������� ���� (���, �� ������)
//fopen(D:/Danya/Libary/C/test1, "r");  // ������� ���� (���, �� ������)

void InitWorkWithFile()
{
    if (!OpenFileForPort())
        return;
    ReadFromFile();


    fclose(file);     // �������� �����
}

void ReadFromFile()
{
    int i;
    int sizeMassive = 10;
    int readM[sizeMassive];
    int read;

    fgets(readM, 8, file);
    //printf("%s\n", readM);

    for(i = 0; i < sizeMassive; i++)
    {
        printf("%s\n", readM);
        printf("%s \n", readM[i]);
    }
    // # ��������
    /*fscanf(file, "%X", &read);
    printf("%X ", read);
    */

    /*char symbol[12];
    symbol = fgetc(file);

    while(1)
    {
        symbol[i] = fgetc(file);
        if(symbol[i] == 0x0E0F)
            return;
    }

    return symbol;*/
}

BOOL OpenFileForPort()
{
    file = fopen("D:/Danya/Libary/C/test1.txt", "r");
    //if ((file = fopen("D:\Danya\Libary\C\test1.txt", "r")) == NULL)
    if (file == NULL)
    {
        printf("Error Open File");
        return TRUE;
    }
    return TRUE;
}

void CloseFileForPort()
{

}
