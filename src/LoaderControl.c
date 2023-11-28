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

#define BREAK_LINE()    printf("//////// ////////\n\n")

uint32_t currentPlisAnswer = 0;

uint32_t sizeFilePlis;
uint32_t addrFilePlisInDk;

uint8_t command[14];
uint8_t commandAnswer[14];

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
        //if (!StartLoadingFile(/*hComm,*/ currentPlis))
            return;
        //if (!WorkCycle())
        /*if (!InitLoaderControl(hComm, currentPlis))
        {
            printf("\nERROR WORK CYCLE\n");
            return;
        }
        //if (!InitWorkWithFile())
        if (!TransmitDataFile(hComm))
        {
            printf("\nERROR WORK FILE\n");
            return;
        }*/
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
    if (!StartConnection(hComm, currentPlis))
    {
        printf("\nERROR WORK CYCLE\n");
        return FALSE;
    }

    currentPlisAnswer = currentPlis;    // Плохой Костыль
    if (!TransmitDataFile())
    {
        printf("\nERROR WORK FILE\n");
        return FALSE;
    }

    return TRUE;
}

BOOL StartConnection(HANDLE hComm, uint32_t currentPlis)
{
    int i;


    FormCommand(command, currentPlis);

    printf("Prepare To Send Data Command\n");
    if(!send_command(command))
    {
        printf("Error send char Command 3\n");
        return FALSE;
    }
    for (i = 0; i < 14; i++)
        printf("\ncommand[%d] =  %x", i, command[i]);
    printf("\n");
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

    if(!CheckAnswerCommand(commandAnswer, currentPlis))
    {
        printf("Error Control Command\n");
        return FALSE;
    }
    return TRUE;
}

BOOL TransmitDataFile()
{
    if (!InitWorkWithFile())
        return FALSE;
    return TRUE;
}

void FormCommand(uint8_t *command, uint32_t currentPlis)
{
    int i;
    uint32_t crc32;
    struct CommandToMk headCommand;

    headCommand.command.bytes.code = K_K_UPR;
    headCommand.command.bytes.plisNumber = currentPlis;

    if (currentPlis != PLIS_CYCLONE)
    {
        for (i = 0; i < 4; i++)
            headCommand.command.bytes.sizeFile[i] = SIZE_PLIS_RUSSIAN >> (i*8);
    }
    else
    {
        for (i = 0; i < 4; i++)
            headCommand.command.bytes.sizeFile[i] = SIZE_PLIS_CYCLONE >> (i*8);
    }

    for (i = 0; i < 4; i++)
        headCommand.command.bytes.addrDk[i] = 0x55;//addrFilePlisInDk >> (i*8);

    crc32 = CRC32(headCommand, 10);
    for (i = 0; i < 4; i++)
        headCommand.command.bytes.Crc32[i] = crc32 >> (i*8);

    for (i = 0; i < 14; i++)
        command[i] = headCommand.command.value[i];
}

BOOL CheckCurrentPlis(uint32_t command, uint32_t *currentPlis)
{
    if (command == 1)
    {
        *currentPlis = PLIS1;
        addrFilePlisInDk = 0;
        //printf("\ncurrent PLIS = %d\n", *currentPlis);
        return TRUE;
    }
    if (command == 2)
    {
        *currentPlis = PLIS2;
        addrFilePlisInDk = SIZE_PLIS_RUSSIAN;
        //printf("current PLIS = %d", *currentPlis);
        return TRUE;
    }
    if (command == 3)
    {
        *currentPlis = PLIS_CYCLONE;
        addrFilePlisInDk = SIZE_PLIS_RUSSIAN * 2;
        return TRUE;
    }
    if (command == 4)
    {
        *currentPlis = PLIS3;
        addrFilePlisInDk = 0;
        return TRUE;
    }
    if (command == 5)
    {
        *currentPlis = PLIS4;
        addrFilePlisInDk = SIZE_PLIS_RUSSIAN;
        return TRUE;
    }
    printf("error current PLIS");
    return FALSE;
}

BOOL CheckAnswerCommand(uint8_t *commandAnswer, uint32_t currentPlis)
{
    int i;
    uint32_t crc32;
    struct AnswerFromMk headAnswer;

    for (i = 0; i < 14; i++)
        headAnswer.answer.value[i] = commandAnswer[i];

    if (headAnswer.answer.bytes.code != K_KVIT)
        return FALSE;
    if (headAnswer.answer.bytes.plisNumber != currentPlis)
        return FALSE;
    if (headAnswer.answer.bytes.emptyBytes != 0x00)
        return FALSE;
    crc32 = CRC32(headAnswer.answer.value, 10);
    if(!CheckCRC32(crc32, headAnswer.answer.bytes.Crc32))
        return FALSE;

    return TRUE;
}


BOOL TransmitPartOfProshivka(uint8_t *dataArray, uint16_t arraySize, uint8_t *answerMk)
{
    uint32_t crc32;

    crc32 = CRC32(dataArray, 512);
    dataArray[512] = crc32;
    dataArray[513] = crc32;
    dataArray[514] = crc32;
    dataArray[515] = crc32;
    if(!send_data(dataArray, arraySize))
    {
        printf("\nError send array\n");
        return FALSE;
    }

    if(!read_data_array_com_port(answerMk))
    {
        printf("Error read com port\n");
        return FALSE;
    }
    if (!CheckAnswerCommand(answerMk, currentPlisAnswer))
        return FALSE;

    return TRUE;
}


