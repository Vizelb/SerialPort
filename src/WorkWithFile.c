
#include "../inc/WorkWithFile.h"

#include <stdio.h>
#include <stdlib.h>


// Description:
// ������ ��� ������ �� ����� ����� ��������� �� ������� "size + 1"


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

char c;
int num;

//fopen(D:\Danya\Libary\C\test1, "r");  // ������� ���� (���, �� ������)
//fopen(D:/Danya/Libary/C/test1, "r");  // ������� ���� (���, �� ������)

void InitWorkWithFile()
{
    int i;

    if (!OpenFileForPort())
        return;

    ReadFromFile();

    GetFileSizeMy();

    fclose(file);     // �������� �����

    printf("\n\n\n\n END");
    /*for(i = 0; i < bufferCounter; i++)
        printf("%02x", buffer[i]);*/



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
    printf("\n%d\n", fileSize);

}

void ReadFromFile()
{
    int i;
    uint8_t stringSize;
    uint32_t sizeRead;
    //char bufferTem[256];
    //char buf = 0;

    while (!feof(file))
    {
        sizeRead = fread(&bufferRead, sizeof(uint8_t), 512, file);
        //i = 0;
        //printf("%c", bufferRead[0]);

        for(i = 0; i < sizeof(bufferRead); i++)
        {
            printf("%x ", bufferRead[i]);   // d - print in hex
            bufferPlis[counterPlis] = bufferRead[i];
            counterPlis++;
        }
        // test format
        //if(bufferRead[1] == 50)//0x32 )
            //printf("\nTRUE\n");

        //printf("\n%d\n", sizeRead);
        if(sizeRead < 512)
        {
            printf("\nEnd of Reading");
            return;
        }
        /*if( feof (file) != 0)
        {
            printf("\nEnd of Reading");
            return;
        }*/
    }
}

BOOL OpenFileForPort()
{
    int sizefiletowrite;
    //int fileSize;
    //file = fopen("D:/Danya/Libary/C/test1.txt", "r");
    file = fopen(proshivkaMk, "rb");

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
