#include "../inc/WorkWithFile.h"
#include "../inc/PortMenu.h"
#include "../inc/LoaderControl.h"

#include <stdio.h>
#include <stdlib.h>


// Description:
// ������ ��� ������ �� ����� ����� ��������� �� ������� "size + 1"


FILE *file;

char proshivkaPlis1[] =         "D:/Danya/Libary/C/MM935_1_1_noise.rbf";
char proshivkaPlis2[] =         "D:/Danya/Libary/C/MM935_2_1_noise.rbf";
char proshivkaPlisCyclone[] =   "D:/Danya/Libary/C/TEST_BA435_PSC.rbf";
char proshivkaPlis3[] =         "D:/Danya/Libary/C/TEST_REKURRENTA_5576.rbf";
char proshivkaPlis4[] =         "D:/Danya/Libary/C/TEST_REKURRENTA_5576.rbf";

uint32_t counterPlis = 0;
uint8_t bufferRead[516];    // ���� 512
uint8_t answerMk[14];       // ���� 12

char c;
int num;

extern switcherPlis;

extern struct CommandDu commandDu;  // CommandConsoleMaker.c

//fopen(D:\Danya\Libary\C\test1, "r");  // ������� ���� (���, �� ������)
//fopen(D:/Danya/Libary/C/test1, "r");  // ������� ���� (���, �� ������)

BOOL InitWorkWithFile(uint32_t currentPlis)
{
    int i;

    if (!OpenFileForPort(currentPlis))
        return FALSE;
    Sleep(1000);
    if (!ReadFromFile())
        return FALSE;

    //GetFileSizeMy();

    fclose(file);     // �������� �����

    printf("\n\n\n\n END OF WORK WRITE/READ FILE");
    /*for(i = 0; i < bufferCounter; i++)
        printf("%02x", buffer[i]);*/

    return TRUE;

}

BOOL InitWorkWithFileDuPoUpdate()
{
    int i;

    if (!OpenFileForPortDuPoUpdate())
        return FALSE;
    Sleep(1000);
    if(!ReadFromFile())
        return FALSE;

    fclose(file);     // �������� �����

    printf("\n\n\n\n END OF WORK WRITE/READ FILE");

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
    uint32_t summaryFileReadInTotal = 0;
    //uint32_t summaryFileTransmit = 0;
    //char bufferTem[256];
    //char buf = 0;
    uint32_t kolvoByte = 0;
    uint32_t numberFullCycle = 0;
    uint32_t numberOstatok = 0;

    int cheZaNumber = 0;

    kolvoByte = GetFileSizeMy();
    numberFullCycle = kolvoByte / 65535;
    numberOstatok = kolvoByte - (numberFullCycle * 65535) ;

     printf("\n\nKolvo Byte = %d", kolvoByte);
     printf("\nNumber Full Cycle = %d", numberFullCycle);
     printf("\nNumber Ostatok = %d", numberOstatok);

    //while (!feof(file))
    for(j = 0; j < numberFullCycle; j++)
    {
        summaryFileRead = 0;
        while(summaryFileRead < 65536)
        {
            cheZaNumber = fseek(file, j*65536 + summaryFileRead, SEEK_SET);
            sizeRead = fread(&bufferRead, sizeof(uint8_t), 512, file);
            summaryFileRead += sizeRead;
            summaryFileReadInTotal += sizeRead;
            /*for(i = 0; i < sizeof(bufferRead); i++)
            {
                //printf("%x ", bufferRead[i]);   // d - print in hex
                //bufferPlis[counterPlis] = bufferRead[i];
                counterPlis++;
            }*/
            printf("\nFile Size = %d, delitel = %d", GetFileSizeMy(), GetFileSizeMy() / 65535);
            printf("\nSumm of Transaction = %d, File Bytes Readed = %d", senderCounter+1, sizeRead);    // ���-�� �������
            printf("\nSumm File Bytes = %d Dec, = %X Hex", summaryFileReadInTotal, summaryFileReadInTotal);
            printf("\nFile Size Ostatok = %d Dec, = %X Hex", kolvoByte - summaryFileReadInTotal, kolvoByte - summaryFileReadInTotal);
            if (!TransmitPartOfProshivka(bufferRead, sizeRead/*sizeof(bufferRead)*/, answerMk))
                return FALSE;

            senderCounter++;
            //summaryFileTransmit += sizeof(bufferRead);

            //printf("\nSize of posilka = %d, and Summary File Transmit = %d", sizeof(bufferRead), summaryFileTransmit);
            //printf("\nSumm of Transaction = %d, Current Addr = %x", senderCounter, counterPlis);
            //printf("\nSumm File Bytes = %d, Skolko prochitali = %d , Addr Current Read = %x", summaryFileRead, sizeRead, summaryFileRead);
            //printf("\nFile Size = , delitel/j = %d",  j);

            /*printf("\nFile Size = %d, delitel = %d", GetFileSizeMy(), GetFileSizeMy() / 65535);
            printf("\nSumm of Transaction = %d", senderCounter);    // ���-�� �������
            printf("\nFile Bytes Readed= %d", sizeRead);
            printf("\nSumm File Bytes = %d", summaryFileRead);*/

            /*if(counterPlis == 0xFFFF)
            {

            }*/
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
        printf("\n\n\n ZASHLI v obrabotchik\n");
        printf("\nFile Size = , delitel/j = %d\n",  j);
        summaryFileRead = 0;

        while(summaryFileRead < numberOstatok)  // !feof(file))
        {
            cheZaNumber = fseek(file, j*65536 + summaryFileRead, SEEK_SET);
            printf("\n Number-shmumber = %d\n", cheZaNumber);
            sizeRead = fread(&bufferRead, sizeof(uint8_t), 512, file);
            summaryFileRead += sizeRead;
            summaryFileReadInTotal += sizeRead;
            /*for(i = 0; i < sizeof(bufferRead); i++)
            {
                //printf("%x ", bufferRead[i]);   // d - print in hex
                //bufferPlis[counterPlis] = bufferRead[i];
                counterPlis++;
            }*/
            /*if (sizeRead != 512)
            {
                printf("\nSkolko prochitali = %d ", sizeRead);
                for(i = 0; i < sizeof(bufferRead); i++)
                    printf("%x ", bufferRead[i]);   // d - print in hex
            }*/
            /*printf("\nFile Size = %d, delitel = %d", GetFileSizeMy(), GetFileSizeMy() / 65535);
            printf("\nSumm of Transaction = %d", senderCounter+1);    // ���-�� �������
            printf("\nFile Bytes Readed= %d", sizeRead);
            printf("\nSumm File Bytes = %d", summaryFileReadInTotal);*/
            printf("\nFile Size = %d, delitel = %d", GetFileSizeMy(), GetFileSizeMy() / 65535);
            printf("\nSumm of Transaction = %d, File Bytes Readed = %d", senderCounter+1, sizeRead);    // ���-�� �������
            printf("\nSumm File Bytes = %d Dec, = %X Hex", summaryFileReadInTotal, summaryFileReadInTotal);
            printf("\nFile Size Ostatok = %d Dec, = %X Hex", kolvoByte - summaryFileReadInTotal, kolvoByte - summaryFileReadInTotal);
            if (!TransmitPartOfProshivka(bufferRead, sizeRead, answerMk))
            {
                printf("ERROR OSTATOK FILE!!");
                return FALSE;
            }
            /*if (sizeRead != 512)
            {
                for(i = 0; i < sizeof(bufferRead); i++)
                    printf("%x ", bufferRead[i]);   // d - print in hex
            }*/


            senderCounter++;
            //summaryFileTransmit += sizeof(bufferRead);
            //printf("\nSize of posilka = %d, and Summary File Transmit = %d", sizeof(bufferRead), summaryFileTransmit);
            //printf("\nSumm of Transaction = %d, Current Addr = %x", senderCounter, counterPlis);
            /*printf("\nNumber Ostatok = %d", numberOstatok);
            printf("\nSumm File Bytes = %d, Skolko prochitali = %d ", summaryFileRead, sizeRead);
            printf("\nFile Size = %d, delitel = %d", GetFileSizeMy(), GetFileSizeMy() / 65535);
            printf("\nFile Size = , delitel/j = %d",  j);*/

            /*printf("\nFile Size = %d, delitel = %d", GetFileSizeMy(), GetFileSizeMy() / 65535);
            printf("\nSumm of Transaction = %d", senderCounter);    // ���-�� �������
            printf("\nFile Bytes Readed= %d", sizeRead);
            printf("\nSumm File Bytes = %d", summaryFileRead);*/

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

BOOL OpenFileForPort(uint32_t currentPlis)
{
    int sizefiletowrite;
    //int fileSize;
    //file = fopen("D:/Danya/Libary/C/test1.txt", "r");
    //file = fopen(proshivkaPlis1, "rb");


    if (currentPlis == PLIS1)
        file = fopen(proshivkaPlis1, "rb");
    else if (currentPlis == PLIS2)
        file = fopen(proshivkaPlis2, "rb");
    else if (currentPlis == PLIS_CYCLONE)
        file = fopen(proshivkaPlisCyclone, "rb");
    else if (currentPlis == PLIS3)
        file = fopen(proshivkaPlis3, "rb");
    else if (currentPlis == PLIS4)
        file = fopen(proshivkaPlis4, "rb");

    else if (currentPlis == ALL_SET1)
    {
        printf("\nSwitcher Plis = %d\n", switcherPlis);
        if (switcherPlis == 0)
        {
            file = fopen(proshivkaPlis1, "rb");
            switcherPlis = 1;
            return TRUE;
        }
        if (switcherPlis == 1)
        {
            file = fopen(proshivkaPlis2, "rb");
            switcherPlis = 2;
            return TRUE;
        }
        if (switcherPlis == 2)
        {
            file = fopen(proshivkaPlisCyclone, "rb");
            switcherPlis = 3;
            return TRUE;
        }
    }

    else if (currentPlis == ALL_SET2)
    {
        if (switcherPlis == 0)
        {
            file = fopen(proshivkaPlis1, "rb");
            switcherPlis = 1;
            return TRUE;
        }
        if (switcherPlis == 1)
        {
            file = fopen(proshivkaPlis2, "rb");
            switcherPlis = 2;
            return TRUE;
        }
        if (switcherPlis == 2)
        {
            file = fopen(proshivkaPlis3, "rb");
            switcherPlis = 3;
            return TRUE;
        }
        if (switcherPlis == 3)
        {
            file = fopen(proshivkaPlis3, "rb");
            switcherPlis = 4;
            return TRUE;
        }
    }

    if (file == NULL)
    {
        printf("Error Open File");
        return TRUE;
    }
    return TRUE;
}

BOOL OpenFileForPortDuPoUpdate()
{
    printf("\n");
    printf("numRPZU = %d\n", commandDu.command.bytes.third_byte.bits.NumRPZU);
    printf("TypePLIS = %d\n", commandDu.command.bytes.four_byte.bits.TypePLIS);
    printf("AMP = %d\n", commandDu.command.bytes.second_byte.bits.AMP);
    printf("NumFileRPZU = %d\n", commandDu.command.bytes.four_byte.bits.NumFileRPZU);
    printf("NumPLIS = %d\n", commandDu.command.bytes.four_byte.bits.NumPLIS);
    printf("AAAAAAAAAAAAAAA\n");
    if (commandDu.command.bytes.third_byte.bits.NumRPZU == 0)   // RPZU 0
    {
        if (commandDu.command.bytes.four_byte.bits.TypePLIS != 0)
            return FALSE;
        if (commandDu.command.bytes.second_byte.bits.AMP == 1 || commandDu.command.bytes.second_byte.bits.AMP == 2)
        {
            if (commandDu.command.bytes.four_byte.bits.NumFileRPZU == 0 && commandDu.command.bytes.four_byte.bits.NumPLIS == 0)
                file = fopen(proshivkaPlis1, "rb");
            if (commandDu.command.bytes.four_byte.bits.NumFileRPZU == 1 && commandDu.command.bytes.four_byte.bits.NumPLIS == 1)
                file = fopen(proshivkaPlis2, "rb");
            if (commandDu.command.bytes.four_byte.bits.NumFileRPZU == 2 && commandDu.command.bytes.four_byte.bits.NumPLIS == 0)
                file = fopen(proshivkaPlis1, "rb");
            if (commandDu.command.bytes.four_byte.bits.NumFileRPZU == 3 && commandDu.command.bytes.four_byte.bits.NumPLIS == 1)
                file = fopen(proshivkaPlis2, "rb");
            if (commandDu.command.bytes.four_byte.bits.NumFileRPZU == 4 && commandDu.command.bytes.four_byte.bits.NumPLIS == 0)
                file = fopen(proshivkaPlis1, "rb");
            if (commandDu.command.bytes.four_byte.bits.NumFileRPZU == 5 && commandDu.command.bytes.four_byte.bits.NumPLIS == 1)
                file = fopen(proshivkaPlis2, "rb");
        }
        if (commandDu.command.bytes.second_byte.bits.AMP == 3)
        {
            if (commandDu.command.bytes.four_byte.bits.NumFileRPZU == 0 && commandDu.command.bytes.four_byte.bits.NumPLIS == 0)
                file = fopen(proshivkaPlis1, "rb");
            if (commandDu.command.bytes.four_byte.bits.NumFileRPZU == 1 && commandDu.command.bytes.four_byte.bits.NumPLIS == 1)
                file = fopen(proshivkaPlis2, "rb");
        }
    }
    if (commandDu.command.bytes.third_byte.bits.NumRPZU == 1) // RPZU 1
    {
        if (commandDu.command.bytes.second_byte.bits.AMP == 1 || commandDu.command.bytes.second_byte.bits.AMP == 2 || commandDu.command.bytes.second_byte.bits.AMP == 3)
        {
            /*if (commandDu.command.bytes.four_byte.bits.TypePLIS != 0)
                return FALSE;*/
            if (commandDu.command.bytes.four_byte.bits.NumFileRPZU == 0 && commandDu.command.bytes.four_byte.bits.NumPLIS == 0)
                file = fopen(proshivkaPlis1, "rb");
            if (commandDu.command.bytes.four_byte.bits.NumFileRPZU == 1 && commandDu.command.bytes.four_byte.bits.NumPLIS == 1)
                file = fopen(proshivkaPlis2, "rb");
            if (commandDu.command.bytes.four_byte.bits.NumFileRPZU == 2 && commandDu.command.bytes.four_byte.bits.NumPLIS == 0)
                file = fopen(proshivkaPlis1, "rb");
            if (commandDu.command.bytes.four_byte.bits.NumFileRPZU == 3 && commandDu.command.bytes.four_byte.bits.NumPLIS == 1)
                file = fopen(proshivkaPlis2, "rb");
        }
        if (commandDu.command.bytes.second_byte.bits.AMP == 1 || commandDu.command.bytes.second_byte.bits.AMP == 2) // Interface Block-A/B
        {
            if (commandDu.command.bytes.four_byte.bits.NumFileRPZU == 4)
                if (commandDu.command.bytes.four_byte.bits.TypePLIS == 1)
                    if (commandDu.command.bytes.four_byte.bits.NumPLIS == 2)
                        file = fopen(proshivkaPlisCyclone, "rb");
        }
        if (commandDu.command.bytes.second_byte.bits.AMP == 3)      // Interface Block-V
        {
            if (commandDu.command.bytes.four_byte.bits.NumFileRPZU != 4)
                return FALSE;
            if (commandDu.command.bytes.four_byte.bits.TypePLIS != 0)
                return FALSE;
            if (commandDu.command.bytes.four_byte.bits.NumPLIS == 2)
                file = fopen(proshivkaPlis3, "rb");
        }
    }

    if (file == NULL)
    {
        printf("\nError Open File\n");
        return FALSE;
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

