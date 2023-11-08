//#include <../inc/WorkWithFile.h>
#include "../inc/WorkWithFile.h"

#include <stdio.h>
#include <stdlib.h>

void reMain();
void ReadBigFile();
// Description:
// Буффер для чтения из файла нужно создавать по размеру "size + 1"


FILE *file;
char name[] = "test1.txt";
char nameTestHex[] = "D:/Danya/Libary/C/test2.txt";
char nameHEX[] = "D:/Danya/Libary/C/ComPortFilesForTest/1986BE91T_DEMO.HEX";
//uint32_t buffer[128];
uint8_t bufferFirst[32];
uint8_t buffer[192518];
char bufferRead[512];
int bufferCounter = 0;
char c;
int num;
int maxn = INT_MIN;

int counterBuff = 0;

//fopen(D:\Danya\Libary\C\test1, "r");  // открыть файл (имя, на чтение)
//fopen(D:/Danya/Libary/C/test1, "r");  // открыть файл (имя, на чтение)

void InitWorkWithFile()
{
    int i;

    if (!OpenFileForPort())
        return;

    ReadFromFile();

    fclose(file);     // закрытие файла

    printf("\n\n\n\n");
    for(i = 0; i < bufferCounter; i++)
        printf("%02x", buffer[i]);


        //ReadBigFile();
}

void ReadBigFile()
{
    char buffer[4096];
    file = fopen(nameTestHex, "rb");
    if(file)
    {
        /*while (fread(buffer, sizeof(char), sizeof(buffer), file) > 0)
        {
            printf("%x", buffer[1]);
        }
        fclose(file);*/

    }
}

void StringParse()
{
    // 1 byte   - size
    // 2-3      - addr
    // 4        - service
    // 5 - size - data
    // 5 + size - ks
    // summ = 5 + size


}

void GetFileSizeMy()
{
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    printf("%d", fileSize);

}

void ReadFromFile()
{
    int i, j, k, n;
    int counter = 0;
    int sizeMassive = 10;
    int readM[sizeMassive];
    int read;
    int crString[32];
    uint32_t readMassive;
    uint32_t sizeRead;

    /*if(fread(bufferRead, 1, 512, file))
    {
        //if(feof(file))
            //printf("Error end file");
        //else printf("Error read file");
    }*/

    /*for (i = 0; i < 512; i++)
    {
        printf("%d", bufferRead[i]);
    }*/


    while (!feof(file))
    {

        /*fscanf(file, "%2hhx", &bufferRead);
        printf("%02x", bufferRead[0]);*/


        // работает как-то криво - не те данные читает
        sizeRead = fread(&bufferRead, 1, 512, file);
        //i = 0;
        //printf("%c", bufferRead[0]);
        for(i = 0; i < sizeof(bufferRead); i++)
            printf("%x ", bufferRead[i]);   // d - print in hex


        printf("\n%d\n", sizeRead);
        if(sizeRead < 512){}
            return;
        if( feof (file) != 0)
        {
            printf("End of Reading");
            return;
        }




        /*if (i == (4 + bufferFirst[0]))
        {
            printf("\nEnter 1\n");
            for (k = 4; k < k + bufferFirst[0]; k++)
            {
                //crString[k] = bufferFirst[k];
                buffer[bufferCounter] = bufferFirst[k];
                bufferCounter++;

            }
        }
        if (i == 1)
            printf("\n");
        if (i == (5 + bufferFirst[0]))
        {
            printf("\nXYZ %02x\n", bufferFirst[0]);
            i = 0;
            continue;
        }*/
    }



    // #5 тоже что-то работает
    /*i = 0;
    while (!feof(file))
    {

        fscanf(file, "%d", &readMassive);

        buffer[counter] = readMassive;
        counter++;
    }

    for (i = 0; i < counter; i ++)
        printf("%x ", buffer[i]);
    printf("\n\n%x\n\n", counter);
    */


    // #4 ???
    /*while (fscand(file, "%c", &c) == 1)
    {
        fprintf(stdout, "%c", c);
    }*/

    // 3. Вероятно работает с ощибкой -- чтение в цикле до конца файла
    /*while (!feof(file))
    {
        if (fread(&buffer, 1, sizeof(buffer), file) > 0)
            printf("%d\n", buffer);
        //c = fgetc(file);
        //fprintf(stdout, "%c", c);
    }
    */

    /* 2. Работает
    fgets(readM, 8, file);
    //printf("%s\n", readM);

    for(i = 0; i < sizeMassive; i++)
    {
        printf("%s\n", readM);
        printf("%s \n", readM[i]);
    }
    */


    // # 1. Работает
    /*fscanf(file, "%X", &read);
    printf("%X ", read);
    */

    /*char symbol[12];
    symbol = fgetc(file);

    while(1)
    {
        symbol[i] = fgetc(file);
        if(symbol[i] == 0xE0F)
            return;
    }

    return symbol;*/
}

BOOL OpenFileForPort()
{
    int sizefiletowrite;
    //int fileSize;
    //file = fopen("D:/Danya/Libary/C/test1.txt", "r");
    file = fopen(nameTestHex, "rb");
    //file = fopen(nameHEX, "r");

    //fileSize = GetFileSize(file, sizefiletowrite);
    //printf("\n\n %d \n\n", fileSize);
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



int ascii_to_hex(char c)
{
    int num = (int) c;
    if (num < 58 && num > 47)
    {
        return num - 48;
    }
    if (num < 103 && num > 96)
    {
        return num - 87;
    }
    return num;
}

void reMain()
{
    unsigned char c1, c2;
    int i = 0;
    int sum;
    OpenFileForPort();

    for (i = 0; i < 6; i++)
    {
        c1 = fgetc(file);
        printf("%d\n", c1);
    }

    /*for (i = 0; i < 15 / 2; i++)
    {
        c1 = ascii_to_hex(fgetc(file));
        c2 = ascii_to_hex(fgetc(file));
        sum = c1 << 4 | c2;

    }*/

}
