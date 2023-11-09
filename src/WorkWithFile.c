
#include "../inc/WorkWithFile.h"
#include "../inc/PortMenu.h"

#include <stdio.h>
#include <stdlib.h>


// Description:
// Буффер для чтения из файла нужно создавать по размеру "size + 1"


FILE *file;
char name[] = "test1.txt";
char nameTestHex[] = "D:/Danya/Libary/C/test1.HEX";
char proshivkaMk[] = "D:/Danya/Libary/C/generateFile";
char proshivkaPlis[] = "D:/Danya/Libary/C/test4.rbf";
char nameHEX[] = "D:/Danya/Libary/C/ComPortFilesForTest/1986BE91T_DEMO.HEX";
//uint32_t buffer[128];

//uint8_t buffer[192518];
uint8_t bufferPlis[77655];
int counterPlis = 0;
uint8_t bufferRead[512];
uint8_t answerMk[12];

char c;
int num;

//fopen(D:\Danya\Libary\C\test1, "r");  // открыть файл (имя, на чтение)
//fopen(D:/Danya/Libary/C/test1, "r");  // открыть файл (имя, на чтение)

BOOL InitWorkWithFile()
{
    int i;

    if (!OpenFileForPort())
        return FALSE;

    if(!ReadFromFile())
        return FALSE;

    //GetFileSizeMy();

    fclose(file);     // закрытие файла

    printf("\n\n\n\n END OF WORK WRITE/READ FILE");
    /*for(i = 0; i < bufferCounter; i++)
        printf("%02x", buffer[i]);*/

    return TRUE;

}


void GetFileSizeMy()
{
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    printf("\n%d\n", fileSize);

}

BOOL ReadFromFile()
{
    int i;
    uint8_t stringSize;
    uint32_t sizeRead;
    //char bufferTem[256];
    //char buf = 0;

    while (!feof(file))
    {
        sizeRead = fread(&bufferRead, sizeof(uint8_t), 512, file);

        for(i = 0; i < sizeof(bufferRead); i++)
        {
            printf("%x ", bufferRead[i]);   // d - print in hex
            bufferPlis[counterPlis] = bufferRead[i];
            counterPlis++;
        }
        TransmitPartOfProshivka(bufferRead, sizeof(bufferRead), answerMk);


        // #test format
        //if(bufferRead[1] == 50)//0x32 )
            //printf("\nTRUE\n");

        //printf("\n%d\n", sizeRead);
        if(answerMk[0] != 0xAA && answerMk[10] != 0x79 && answerMk[11] != 0x75)
            return FALSE;
        if(sizeRead < 512)
        {
            printf("\nEnd of Reading");
            return FALSE;
        }
        /*if( feof (file) != 0)
        {
            printf("\nEnd of Reading");
            return;
        }*/
    }
    return TRUE;
}

BOOL OpenFileForPort()
{
    int sizefiletowrite;
    //int fileSize;
    //file = fopen("D:/Danya/Libary/C/test1.txt", "r");
    //file = fopen(proshivkaMk, "rb");
    file = fopen(proshivkaPlis, "rb");

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


int hex_to_int(char c)
{
    int first = c / 16 -3;
    int second = c % 16;
    int result = first * 10 + second;
    if (result > 9)
        result--;
    return result;
}

int hex_to_ascii(char c, char d)
{
    int high = hex_to_int(c) * 16;
    int low = hex_to_int(d);
    return high + low;
}

int hexChrBin(const char hex, char *out)
{
    if(out == NULL)
        return 0;

    if (hex >= '0' && hex <= '9')
        *out = hex - '0';
    else if (hex >= 'A' && hex <= 'F')
        *out = hex - 'A' + 10;
    else if (hex >= 'a' && hex <= 'f')
        *out = hex - 'a' + 10;
    else return 0;

    return 1;

}

void to_hex_16(char *output, unsigned n)
{
    static const char hex_digit[] = "0123456789abcdef";



    output[0] = hex_digit[(n >> 12) & 0xf];
    output[1] = hex_digit[(n >> 8) & 0xf];
    output[2] = hex_digit[(n >> 4) & 0xf];
    output[3] = hex_digit[n & 0xf];
    output[4] = '\0';
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


