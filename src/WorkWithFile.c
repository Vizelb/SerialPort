#include "../inc/WorkWithFile.h"
#include "../inc/PortMenu.h"
#include "../inc/LoaderControl.h"

#include <stdio.h>
#include <stdlib.h>


// Description:
// Буффер для чтения из файла нужно создавать по размеру "size + 1"


FILE *file;

char proshivkaPlis1[] =         "D:/Danya/Libary/C/935_1_2.rbf";
char proshivkaPlis2[] =         "D:/Danya/Libary/C/935_2_2.rbf";
char proshivkaPlisCyclone[] =   "D:/Danya/Libary/C/TEST_BA435_PSC.rbf";
char proshivkaPlis3[] =         "D:/Danya/Libary/C/TEST_REKURRENTA_5576.rbf";
char proshivkaPlis4[] =         "D:/Danya/Libary/C/TEST_REKURRENTA_5576.rbf";

uint32_t counterPlis = 0;
uint8_t bufferRead[516];    // было 512
uint8_t answerMk[14];       // было 12

char c;
int num;

extern switcherPlis;

extern struct CommandDu commandDu;  // CommandConsoleMaker.c

//fopen(D:\Danya\Libary\C\test1, "r");  // открыть файл (имя, на чтение)
//fopen(D:/Danya/Libary/C/test1, "r");  // открыть файл (имя, на чтение)

// Используется для загрузки в ДК
BOOL InitWorkWithFile(uint32_t currentPlis)
{
    if (!OpenFileForPort(currentPlis))
        return FALSE;
    Sleep(1000);
    if (!ReadFromFile())
        return FALSE;
    fclose(file);     // закрытие файла
    printf("\n\n\n\n END OF WORK WRITE/READ FILE");
    return TRUE;

}


BOOL WorkWithFileDuPoUpdate()
{
    if (!OpenFilePLISDuPoUpdate())
        return FALSE;
    Sleep(1000);
    if (!ReadFromFile())
        return FALSE;
    fclose(file);     // закрытие файла
    printf("\n\n\n\n END OF WORK WRITE/READ FILE");
    return TRUE;

}

// Ф-ция по чтению и отправки файла на com-port по 512 байт + 4 байта КС
BOOL ReadFromFile()
{
    int i, j;
    uint8_t stringSize;
    uint32_t sizeRead;
    uint32_t senderCounter = 0;
    uint32_t summaryFileRead = 0;
    uint32_t summaryFileReadInTotal = 0;
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

    for(j = 0; j < numberFullCycle; j++)
    {
        summaryFileRead = 0;
        while(summaryFileRead < 65536)
        {
            cheZaNumber = fseek(file, j*65536 + summaryFileRead, SEEK_SET);
            sizeRead = fread(&bufferRead, sizeof(uint8_t), 512, file);
            summaryFileRead += sizeRead;
            summaryFileReadInTotal += sizeRead;

            printf("\nFile Size = %d, delitel = %d", GetFileSizeMy(), GetFileSizeMy() / 65535);
            printf("\nSumm of Transaction = %d, File Bytes Readed = %d", senderCounter+1, sizeRead);    // Кол-во посылок
            printf("\nSumm File Bytes = %d Dec, = %X Hex", summaryFileReadInTotal, summaryFileReadInTotal);
            printf("\nFile Size Ostatok = %d Dec, = %X Hex", kolvoByte - summaryFileReadInTotal, kolvoByte - summaryFileReadInTotal);
            if (!TransmitPartOfProshivka(bufferRead, sizeRead/*sizeof(bufferRead)*/, answerMk))
                return FALSE;
            senderCounter++;

            if(sizeRead < 512)
            {
                printf("\n ERROR End of Reading");
                return FALSE;
            }
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

            printf("\nFile Size = %d, delitel = %d", GetFileSizeMy(), GetFileSizeMy() / 65535);
            printf("\nSumm of Transaction = %d, File Bytes Readed = %d", senderCounter+1, sizeRead);    // Кол-во посылок
            printf("\nSumm File Bytes = %d Dec, = %X Hex", summaryFileReadInTotal, summaryFileReadInTotal);
            printf("\nFile Size Ostatok = %d Dec, = %X Hex", kolvoByte - summaryFileReadInTotal, kolvoByte - summaryFileReadInTotal);
            if (!TransmitPartOfProshivka(bufferRead, sizeRead, answerMk))
            {
                printf("ERROR OSTATOK FILE!!");
                return FALSE;
            }
            senderCounter++;

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

BOOL TransmitPartOfProshivka(uint8_t *dataArray, uint16_t arraySize, uint8_t *answerMk)
{
    int i;
    uint32_t crc32;

    printf("\nArray Size To Transmit = %d\n", arraySize + 4);
    crc32 = CRC32(dataArray, arraySize);
    for (i = 0; i < 4; i++)
        dataArray[arraySize + i] = crc32 >> (i*8);

    for (i = 0; i < arraySize+4; i++)
        printf("[%3.d] = %2.x  ", i, dataArray[i]);
    printf("\n");

    Sleep(80);

    if (!TxRx_DataControl(dataArray, arraySize + 4, READ_TIME_DATA))
        return FALSE;

    return TRUE;
}

// Ф-ция для открытия файла прошивки ПЛИС
BOOL OpenFilePLISDuPoUpdate()
{
    printf("\n");
    printf("numRPZU = %d\n", commandDu.command.bytes.third_byte.bits.NumRPZU);
    printf("TypePLIS = %d\n", commandDu.command.bytes.four_byte.bits.TypePLIS);
    printf("AMP = %d\n", commandDu.command.bytes.second_byte.bits.AMP);
    printf("NumFileRPZU = %d\n", commandDu.command.bytes.four_byte.bits.NumFileRPZU);
    printf("NumPLIS = %d\n", commandDu.command.bytes.four_byte.bits.NumPLIS);
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

BOOL OpenFileForPort(uint32_t currentPlis)
{
    //file = fopen("D:/Danya/Libary/C/test1.txt", "r");
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
        return FALSE;
    }
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






