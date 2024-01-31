#include "../inc/LoaderControl.h"
#include "../inc/PortMenu.h"
#include "../inc/WorkWithFile.h"
#include "../inc/ConsoleControl.h"
#include "../inc/CommandConsoleMaker.h"

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
uint8_t plisNum;
uint8_t plisType;
uint8_t rpzuFileNumber;

uint8_t currentBlock;

#define BREAK_LINE()    printf("//////// ////////\n\n")
extern HANDLE hComm;

uint32_t currentPlisAnswer = 0;

uint32_t sizeFilePlis;
uint32_t addrFilePlisInDk;
//uint32_t sizeFilePlisInDk;

uint8_t command[14];
uint8_t commandAnswer[14];

int switcherPlis = 0;

uint32_t commandPlis;

// Главная ф-ция программы
void newMainFunc(void)
{
    int i;
    int consoleCommand;

    printf("START\n");
    while(1)
    {
        consoleCommand = DuConsoleCommand();
        if (consoleCommand == 9)
            break;

        //CommandConsoleMaker(consoleCommand, command);

        /*if (!SetSettingsComPort(COM_PORT_4, CBR_115200))
            return;
        if (consoleCommand == 1 || consoleCommand == 2)
            CommandLoadDuPoPlis();
        if (consoleCommand == 3)
            InitLoaderControl();
        close_com_port();*/
        if (!OperatingModeController(consoleCommand)) {
            printf("\nWork Error!\n");
            break;
        }
    }
    printf("END\n");
}

BOOL OperatingModeController(int consoleCommand)
{
    if (consoleCommand == 1 || consoleCommand == 2)
        return CommandLoadDuPoPlis();
    if (consoleCommand == 3)
        InitLoaderControl();
}

// Загрузка ПО ПЛИС по протоколу ДУ
BOOL CommandLoadDuPoPlis()
{
    int i;
    uint8_t command[14];
    uint8_t commandAnswer[14];

    FormCommandLoadDuPoPlis(command);

    if(!SendData(command, 14))
        return FALSE;

    if(!ReadData(commandAnswer, 14, READ_TIME_COMMAND))
        return FALSE;
    // Контроль ответа?? - КС, ...

    printf("Start Loading File \n");
    if (!WorkWithFileDuPoUpdate())
        return FALSE;

    return TRUE;
}


// Загрузчик ПО ПЛИС в ДК
void InitLoaderControl()
{
    int i, j;
    uint32_t currentPlis;
    //uint8_t command[14];
    //uint8_t commandAnswer[14];
    //uint32_t commandPlis;

    BREAK_LINE();

    // #3
    printf("Start Work\n");
    //commandPlis = ConsoleCommandDkDriverUpdate();
    if(CheckCurrentPlis(commandPlis, &currentPlis))
    {
        if (!StartLoadingFile(hComm, currentPlis))
        {
            printf("Che za error?");
            return;
        }
    }
    else printf("ERROR COMMAND");

}


BOOL StartLoadingFile(HANDLE hComm, uint32_t currentPlis)
{
    if (!TransmitCommandControl(hComm, currentPlis))
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

BOOL TransmitCommandControl(HANDLE hComm, uint32_t currentPlis)
{
    int i;

    FormCommandDkDriverUpdate(command, currentPlis);

    printf("Prepare To Send Data Command\n");
    if(!SendData(command, 14))
        return FALSE;

    /*for (i = 0; i < 14; i++)
        printf("\ncommand[%d] =  %x", i, command[i]);
    printf("\n");*/

    //CancelFunctiontIoEx();

    if(!ReadData(commandAnswer, 14, READ_TIME_COMMAND))
        return FALSE;

    //CancelFunctiontIoEx();

    if(!CheckAnswerCommand(commandAnswer, /*currentPlis,*/ K_KVIT))
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



/*void FormCommandDu(uint8_t *command, uint32_t currentPlis, uint8_t codeCommand)
{
    int i;
    uint32_t crc32;
    struct CommandDu duCommand;

    duCommand.command.bytes.code = codeCommand;
    duCommand.command.bytes.second_byte.value = 0x00;    //sbp;      currentPlis;

    duCommand.command.bytes.third_byte.bits.NumRPZU = rpzuNumber;
    duCommand.command.bytes.third_byte.bits.EMPTY = 0x00;

    //duCommand.command.bytes.four_byte.bits.NumPLIS = plisNumber;
    duCommand.command.bytes.four_byte.bits.TypePLIS = plisType;
    duCommand.command.bytes.four_byte.bits.NumFileRPZU = rpzuFileNumber;


    crc32 = CRC32(duCommand, 10);
    for (i = 0; i < 4; i++)
        duCommand.command.bytes.Crc32[i] = crc32 >> (i*8);

    for (i = 0; i < 14; i++)
        command[i] = duCommand.command.value[i];
}*/

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

BOOL CheckAnswerCommand(uint8_t *commandAnswer, /*uint32_t currentPlis,*/ uint8_t codeCommand)
{
    int i;
    uint32_t crc32;
    struct AnswerFromMk headAnswer;

    for (i = 0; i < 14; i++)
        headAnswer.answer.value[i] = commandAnswer[i];

    if (headAnswer.answer.bytes.code != codeCommand)
        return FALSE;
    /*if (headAnswer.answer.bytes.plisNumber != currentPlis)
        return FALSE;*/
    for (i = 0; i < 7; i++)
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


void CheckCommandControl(int consoleCommand)
{

    uint32_t currentCommand;

    if ((consoleCommand >> 8) == 1)         // 16-31
        currentBlock = 1;
    else if ((consoleCommand >> 8) == 2)    // 32-47
        currentBlock = 2;
    else if ((consoleCommand >> 8) == 3)    // 48-63
        currentBlock = 3;
    else return;

    currentCommand = consoleCommand & 0x0F;

    if (currentBlock == 1)
    {
        if (currentCommand == 0)        // РС
        {
            rpzuNumber = 0;
            //plisNumber = 0;         // ПЛИС 1 или 2
            plisType = 0;           // Отечка
            rpzuFileNumber = 0;
        }
        if (currentCommand == 1)        // ШС
        {
            rpzuNumber = 0;
            //plisNumber = 1;         // ПЛИС 1 или 2
            plisType = 0;           // Отечка
            rpzuFileNumber = 1;
        }
        if (currentCommand == 2)        // Интерфейс А и Б - Циклон
        {
            rpzuNumber = 0;
            //plisNumber = 2;         // ПЛИС 2
            plisType = 1;           // Циклон
            rpzuFileNumber = 2;
        }
        else return;
    }
    if (currentBlock == 2)
    {

    }
    if (currentBlock == 3)
    {
        if (currentCommand == 0)        // РСИ
        {
            rpzuNumber = 0;
            //plisNumber = 0;         // ПЛИС 1 или 2
            plisType = 0;           // Отечка
            rpzuFileNumber = 0;
        }
        if (currentCommand == 1)        // ШСИ
        {
            rpzuNumber = 0;
            //plisNumber = 1;         // ПЛИС 1 или 2
            plisType = 0;           // Отечка
            rpzuFileNumber = 1;
        }
        if (currentCommand == 2)        // Интерфейс А и Б - Циклон
        {
            rpzuNumber = 0;
            //plisNumber = 2;         // ПЛИС 1 или 2
            plisType = 0;           // Отечка
            rpzuFileNumber = 2;
        }
        if (currentCommand == 3)        // Интерфейс А и Б - Циклон
        {
            rpzuNumber = 0;
            //plisNumber = 3;         // ПЛИС 1 или 2
            plisType = 0;           // Отечка
            rpzuFileNumber = 2;
        }
    }

}










/*void testUartFunc(void)
{
    int i;
    uint8_t consoleCommand = 0;
    int consoleCommand2;
    uint8_t data[14];
    uint8_t dataAnswer[14];

    for (i = 0; i < 14; i++)
        data[i] = 0xdc;

    printf("START\n");

    if (!SetSettingsComPort(COM_PORT_9, CBR_9600))
        return;

    while(1)
    {
        //consoleCommand2 = testCommand();
        if (consoleCommand == 1)
        {
            //consoleCommand = 0;
            printf("Send 0xAA\n");
            for (i = 0; i < 14; i++)
                data[i] = 0xAA;
        }
        if (consoleCommand == 2)
        {
            consoleCommand = 0;
            printf("Send 0x55\n");
            for (i = 0; i < 14; i++)
                data[i] = 0x55;
        }
        printf("Prepare Send data\n");

        if(!SendData(data, 14))
        {
            printf("\nError send array\n");
            return FALSE;
        }

        if(!ReadData(dataAnswer, 14, READ_TIME_DATA))
        {
            printf("Error read com port\n");
            return FALSE;
        }
        for (i = 0; i < 14; i++)
            printf("[%d] = %d ", i, dataAnswer[i]);
        printf("\n");

        if (consoleCommand2 == 9)
            break;

        consoleCommand++;

    }


    printf("Prepare To Close Port\n");
    close_com_port();
    printf("Successfully Closed Port!\n");

    printf("END\n");
}*/












