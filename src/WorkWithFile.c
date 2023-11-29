#include "../inc/WorkWithFile.h"
#include "../inc/PortMenu.h"
#include "../inc/LoaderControl.h"

#include <stdio.h>
#include <stdlib.h>


// Description:
// Буффер для чтения из файла нужно создавать по размеру "size + 1"


FILE *file;
char nameTxt[] = "test3.txt";
char nameTestHex[] = "D:/Danya/Libary/C/test1.HEX";
char proshivkaMk[] = "D:/Danya/Libary/C/generateFile";
char imitPlis[] = "D:/Danya/Libary/C/imitPlis";
char proshivkaPlis[] = "D:/Danya/Libary/C/test4.rbf";
char testNewFile[] = "D:/Danya/Libary/C/testNewFile.txt";
char nameHEX[] = "D:/Danya/Libary/C/ComPortFilesForTest/1986BE91T_DEMO.HEX";
//uint32_t buffer[128];

//uint8_t buffer[192518];
//uint8_t bufferPlis[77655];
uint32_t counterPlis = 0;
uint8_t bufferRead[516];    // было 512
uint8_t answerMk[14];       // было 12

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


uint32_t GetFileSizeMy()
{
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    printf("\n%d\n", fileSize);
    return fileSize;
}

BOOL ReadFromFile()
{
    int i, j;
    uint8_t stringSize;
    uint32_t sizeRead;

    uint32_t senderCounter = 0;
    uint32_t summaryFileRead = 0;
    //char bufferTem[256];
    //char buf = 0;
    uint32_t kolvoByte = 0;
    uint32_t numberFullCycle = 0;
    uint32_t numberOstatok = 0;

    int cheZaNumber = 0;

    kolvoByte = GetFileSizeMy();
    numberFullCycle = kolvoByte / 65535;
    numberOstatok = kolvoByte - (numberFullCycle * 65535) ;

     printf("\nKolvo Byte = %d", kolvoByte);
     printf("\nNumber Full Cycle = %d", numberFullCycle);
     printf("\nNumber Ostatok = %d", numberOstatok);

    //while (!feof(file))
    for(j = 0; j < numberFullCycle; j++)
    {
        summaryFileRead = 0;
        while(summaryFileRead < 65536)
        {
            cheZaNumber= fseek(file, j*65536+summaryFileRead, SEEK_SET);
            sizeRead = fread(&bufferRead, sizeof(uint8_t), 512, file);
            summaryFileRead += sizeRead;
            for(i = 0; i < sizeof(bufferRead); i++)
            {
                //printf("%x ", bufferRead[i]);   // d - print in hex
                //bufferPlis[counterPlis] = bufferRead[i];
                counterPlis++;
            }
            if (!TransmitPartOfProshivka(bufferRead, sizeof(bufferRead), answerMk))
                return FALSE;
            senderCounter++;
            printf("\nSumm of Transaction = %d, Current Addr = %x", senderCounter, counterPlis);
            printf("\nSumm File Bytes = %d, Skolko prochitali = %d ", summaryFileRead, sizeRead);
            printf("\nFile Size = , delitel/j = %d",  j);
            if(counterPlis == 0xFFFF)
            {

            }
        // #test format
        //if(bufferRead[1] == 50)//0x32 )
            //printf("\nTRUE\n");

        //printf("\n%d\n", sizeRead);
            if(sizeRead < 512)
            {
                printf("\n ERROR End of Reading");
                return FALSE;
            }
        /*if( feof (file) != 0)
        {
            printf("\nEnd of Reading");
            return;
        }*/
        }
    }

    if (numberOstatok != 0)
    {
        printf("\n ZASHLI v obrabotchik\n");
        printf("\nFile Size = , delitel/j = %d\n",  j);
        summaryFileRead = 0;

        while(summaryFileRead < numberOstatok)//!feof(file))
        {
            cheZaNumber= fseek(file, j*65536+summaryFileRead, SEEK_SET);
            printf("\n Number-shmumber = %d\n", cheZaNumber);
            sizeRead = fread(&bufferRead, sizeof(uint8_t), 512, file);
            summaryFileRead += sizeRead;
            for(i = 0; i < sizeof(bufferRead); i++)
            {
                printf("%x ", bufferRead[i]);   // d - print in hex
                //bufferPlis[counterPlis] = bufferRead[i];
                counterPlis++;
            }
            if (!TransmitPartOfProshivka(bufferRead, sizeRead, answerMk))
                return FALSE;
            senderCounter++;
            printf("\nNumber Ostatok = %d", numberOstatok);
            printf("\nSize of posilka = %d, and i = %d", sizeof(bufferRead), i);
            printf("\nSumm of Transaction = %d, Current Addr = %x", senderCounter, counterPlis);
            printf("\nSumm File Bytes = %d, Skolko prochitali = %d ", summaryFileRead, sizeRead);
            printf("\nFile Size = %d, delitel = %d", GetFileSizeMy(), GetFileSizeMy() / 65535);
            printf("\nFile Size = , delitel/j = %d",  j);

            if(sizeRead < 512)
            {
                printf("\n End of Reading tyta chtoli?");
                break;
            }
        }
    }

    printf("END OF FILE");
    return TRUE;
}

BOOL OpenFileForPort()
{
    int sizefiletowrite;
    //int fileSize;
    //file = fopen("D:/Danya/Libary/C/test1.txt", "r");
    //file = fopen(proshivkaMk, "rb");
    //file = fopen(imitPlis, "rb");
    file = fopen(proshivkaPlis, "rb");
    //file = fopen(testNewFile, "rb");
    //file = fopen(nameTxt, "rb");

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



/*int ascii_to_hex(char c)
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
*/

