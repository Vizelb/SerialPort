#include "../inc/LoaderControl.h"
#include "../inc/PortMenu.h"
#include "../inc/WorkWithFile.h"

#include <stdio.h>
#include <stdlib.h>

/*#include <conio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#include <inttypes.h>

#include <winbase.h>
#include <windows.h>

#include <stdbool.h>*/

uint8_t rpzuNumber;
uint8_t plisNumber;
uint8_t plisType;
uint8_t rpzuFileNumber;

#define BREAK_LINE()    printf("//////// ////////\n\n")
extern HANDLE hComm;

uint32_t currentPlisAnswer = 0;

uint32_t sizeFilePlis;
uint32_t addrFilePlisInDk;
//uint32_t sizeFilePlisInDk;

uint8_t command[14];
uint8_t commandAnswer[14];

int switcherPlis = 0;

void InitLoaderControl()
{
    int i, j;
    uint32_t currentPlis;
    uint32_t commandPlis;

    printf("Start\n");
    BREAK_LINE();

    // #1
    printf("Try to open port\n");
      if (!init_com_port())
    {
        printf("Error1\n");
        return;
    }
    BREAK_LINE();

    // #2
    printf("Try to configurate port\n");
    if(!config_com_port())
    {
        printf("Error2\n");
        return;
    }
    BREAK_LINE();

    // #3
    printf("Start Work\n");
    commandPlis = InputCommand();
    if(CheckCurrentPlis(commandPlis, &currentPlis))
    {
        if (!StartLoadingFile(hComm, currentPlis))
        {
            printf("Che za error?");
            return;
        }
    }
    else printf("ERROR COMMAND");

    // #4
    BREAK_LINE();
    printf("Prepare To Close Port\n");
    close_com_port();
    printf("End\n");
    BREAK_LINE();

}


BOOL StartLoadingFile(HANDLE hComm, uint32_t currentPlis)
{
    if (!TransmitCommandControl(hComm, currentPlis, K_K_UPR))
    {
        printf("\nERROR WORK CYCLE\n");
        return FALSE;
    }

    currentPlisAnswer = currentPlis;    // Ïëîõîé Êîñòûëü
    if (!TransmitDataFile(currentPlis))
    {
        printf("\nERROR WORK FILE\n");
        return FALSE;
    }

    /*if (!TransmitCommandControl(hComm, currentPlis, K_K_END))
    {
        printf("\nERROR WORK CYCLE\n");
        return FALSE;
    }*/

    return TRUE;
}

BOOL TransmitCommandControl(HANDLE hComm, uint32_t currentPlis, uint8_t codeCommand)
{
    int i;

    FormCommand(command, currentPlis, codeCommand);

    //else FormCommandDu(command, currentPlis, codeCommand);

    printf("Prepare To Send Data Command\n");
    if(!send_command(command))
    {
        printf("Error send char Command 3\n");
        return FALSE;
    }
    /*for (i = 0; i < 14; i++)
        printf("\ncommand[%d] =  %x", i, command[i]);
    printf("\n");*/
    printf("Command Send\n");

    CancelIoEx(hComm, NULL);
    //CancelFunctiontIoEx();

    printf("Prepare To Read\n");
    if(!read_command_com_port(commandAnswer))
    {
        printf("Error read com port Command\n");
        return FALSE;
    }

    CancelIoEx(hComm, NULL);
    //CancelFunctiontIoEx();

    if(!CheckAnswerCommand(commandAnswer, currentPlis, K_KVIT))
    {
        printf("Error Control Command\n");
        return FALSE;
    }
    return TRUE;
}

BOOL TransmitDataFile(uint32_t currentPlis)
{
    if (currentPlis == ALL_SET1)
    {
        while (switcherPlis != 3)
        {
            //printf("First cyrcle error - out");
            if (!InitWorkWithFile(currentPlis))
            {
                printf("First cyrcle error");
                return FALSE;
            }
        }
    }
    else if (currentPlis == ALL_SET2)
    {
        while (switcherPlis != 4)
        {
            if (!InitWorkWithFile(currentPlis))
                return FALSE;
        }
    }
    else if (!InitWorkWithFile(currentPlis))
        return FALSE;

    return TRUE;
}

void FormCommand(uint8_t *command, uint32_t currentPlis, uint8_t codeCommand)
{
    int i;
    uint32_t crc32;
    struct CommandToMk headCommand;

    headCommand.command.bytes.code = codeCommand;
    headCommand.command.bytes.plisNumber = currentPlis;

    for (i = 0; i < 4; i++)
        headCommand.command.bytes.sizeFile[i] = sizeFilePlis >> (i*8);

    for (i = 0; i < 4; i++)
        headCommand.command.bytes.addrDk[i] = addrFilePlisInDk >> (i*8);

    crc32 = CRC32(headCommand, 10);
    for (i = 0; i < 4; i++)
        headCommand.command.bytes.Crc32[i] = crc32 >> (i*8);

    for (i = 0; i < 14; i++)
        command[i] = headCommand.command.value[i];
}

void FormCommandDu(uint8_t *command, uint32_t currentPlis, uint8_t codeCommand)
{
    int i;
    uint32_t crc32;
    struct CommandDu duCommand;

    duCommand.command.bytes.code = codeCommand;
    duCommand.command.bytes.sec_byte = 0x00;    //sbp;      currentPlis;

    duCommand.command.bytes.third_byte.bits.NumRPZU = rpzuNumber;
    duCommand.command.bytes.third_byte.bits.EMPTY = 0x00;

    duCommand.command.bytes.four_byte.bits.NumPLIS = plisNumber;
    duCommand.command.bytes.four_byte.bits.TypePLIS = plisType;
    duCommand.command.bytes.four_byte.bits.NumFileRPZU = rpzuFileNumber;


    crc32 = CRC32(duCommand, 10);
    for (i = 0; i < 4; i++)
        duCommand.command.bytes.Crc32[i] = crc32 >> (i*8);

    for (i = 0; i < 14; i++)
        command[i] = duCommand.command.value[i];
}

BOOL CheckCurrentPlis(uint32_t command, uint32_t *currentPlis)
{
    if (command == 1)
    {
        *currentPlis = PLIS1;
        addrFilePlisInDk = 0;
        sizeFilePlis = SIZE_PLIS_RUSSIAN;
        //printf("\ncurrent PLIS = %d\n", *currentPlis);


        return TRUE;
    }
    if (command == 2)
    {
        *currentPlis = PLIS2;
        addrFilePlisInDk = SIZE_PLIS_RUSSIAN;
        sizeFilePlis = SIZE_PLIS_RUSSIAN;
        //printf("current PLIS = %d", *currentPlis);
        return TRUE;
    }
    if (command == 3)
    {
        *currentPlis = PLIS_CYCLONE;
        addrFilePlisInDk = SIZE_PLIS_RUSSIAN * 2;
        sizeFilePlis = SIZE_PLIS_CYCLONE;
        return TRUE;
    }
    if (command == 4)
    {
        *currentPlis = PLIS3;
        addrFilePlisInDk = SIZE_PLIS_RUSSIAN * 2;
        sizeFilePlis = SIZE_PLIS_RUSSIAN;
        return TRUE;
    }
    if (command == 5)
    {
        *currentPlis = PLIS4;
        addrFilePlisInDk = SIZE_PLIS_RUSSIAN * 2 + SIZE_PLIS_RUSSIAN;
        sizeFilePlis = SIZE_PLIS_RUSSIAN;
        return TRUE;
    }

    if (command == 6)   // ÄËß ÂÑÅÕ ÂÎÇÌÎÆÍÛÕ ÏËÈÑ ÄËß ÄÊ
    {
        *currentPlis = ALL_SET1;
        addrFilePlisInDk = 0;
        sizeFilePlis = SIZE_PLIS_RUSSIAN;
        return TRUE;
    }
    if (command == 7)   // ÄËß ÂÑÅÕ ÂÎÇÌÎÆÍÛÕ ÏËÈÑ ÄËß ÄÊ
    {
        *currentPlis = ALL_SET2;
        addrFilePlisInDk = 0;
        sizeFilePlis = SIZE_PLIS_RUSSIAN;
        return TRUE;
    }

    printf("error current PLIS");
    return FALSE;
}

BOOL CheckAnswerCommand(uint8_t *commandAnswer, uint32_t currentPlis, uint8_t codeCommand)
{
    int i;
    uint32_t crc32;
    struct AnswerFromMk headAnswer;

    for (i = 0; i < 14; i++)
        headAnswer.answer.value[i] = commandAnswer[i];

    if (headAnswer.answer.bytes.code != codeCommand)
        return FALSE;
    if (headAnswer.answer.bytes.plisNumber != currentPlis)
        return FALSE;
    for (i = 0; i < 8; i++)
        if (headAnswer.answer.bytes.emptyBytes[i] != 0x00){
            printf("error 3");
            return FALSE;
        }
    crc32 = CRC32(headAnswer.answer.value, 10);
    if(!CheckCRC32(crc32, headAnswer.answer.bytes.Crc32)){
        printf("error 4");
        return FALSE;
    }

    return TRUE;
}


BOOL TransmitPartOfProshivka(uint8_t *dataArray, uint16_t arraySize, uint8_t *answerMk)
{
    int i;
    uint32_t crc32;
    uint16_t sizeToTransmit = arraySize;

    if (arraySize != 512)
    {
        for (i = arraySize; i < 512; i++)
            dataArray[i] = 0x00;
    }

    /*for (i = 0; i < 516; i++)
        printf("\ncommand[%d] =  %x", i, dataArray[i]);
    printf("\n");*/

    printf("\nArray Size = %d", arraySize);
    printf("\nArray Size = %d", sizeToTransmit);
    crc32 = CRC32(dataArray, 512);
    for (i = 0; i < 4; i++)
        dataArray[512 + i] = crc32 >> (i*8);


    /*for (i = 0; i < 516; i++)
        printf("\ncommand[%d] =  %x", i, dataArray[i]);
    printf("\n");*/

    if(!send_data(dataArray, sizeToTransmit + 4))
    {
        printf("\nError send array\n");
        return FALSE;
    }
    printf("\n  1  No ERROR send array\n");
    if(!read_data_array_com_port(answerMk))
    {
        printf("Error read com port\n");
        return FALSE;
    }
    printf("\n  2  No ERROR send array\n");
    if (!CheckAnswerCommand(answerMk, currentPlisAnswer, K_R_D))
    {
        printf("Error read com port DATA - check bytes\n");
        return FALSE;
    }
    printf("\n 3  No ERROR send array\n");
    return TRUE;
}


