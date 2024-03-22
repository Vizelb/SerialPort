#include "../inc/LoaderControl.h"
#include "../inc/PortMenu.h"
#include "../inc/WorkWithFile.h"
#include "../inc/ConsoleControl.h"
#include "../inc/CommandConsoleMaker.h"
#include "../inc/DkWork.h"

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

//uint8_t command[14];
//uint8_t commandAnswer[14];

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

        if (!OperatingModeController(consoleCommand)) {
            printf("\nWork Error!\n");
            break;
        }
        close_com_port();
    }
    //close_com_port();
    printf("END\n");
}

BOOL OperatingModeController(int consoleCommand)
{
    printf("Start Work\n");

    if (consoleCommand == 1 || consoleCommand == 2)
        return LoadByDuPoPlisFirmware();
    if (consoleCommand == 3)
        return LoadInDkPlisFirmware();
}

// Загрузка ПО ПЛИС по протоколу ДУ
BOOL LoadByDuPoPlisFirmware()
{
    int i;
    uint8_t command[14];

    if (!SetSettingsComPort(COM_PORT_4, CBR_115200))
            return FALSE;

    FormCommandLoadDuPoPlis(command);

    if (!TxRx_DataControl(command, 14, READ_TIME_COMMAND))
        return FALSE;

    if (!WorkWithFileDuPoUpdate())
        return FALSE;

    return TRUE;
}




BOOL TxRx_DataControl(uint8_t *command, uint32_t commandSize, int time_out)
{
    uint8_t commandAnswer[14];

    if(!SendData(command, commandSize))
        return FALSE;
    if(!ReadData(commandAnswer, 14, time_out))
        return FALSE;
    if(!CheckAnswerCommand(commandAnswer, K_KVIT))
        return FALSE;
    return TRUE;
}

BOOL CheckAnswerCommand(uint8_t *commandAnswer, uint8_t codeCommand)
{
    int i;
    uint32_t crc32;
    struct AnswerFromMk headAnswer;

    for (i = 0; i < 14; i++)
        headAnswer.answer.value[i] = commandAnswer[i];

    if (headAnswer.answer.bytes.code != codeCommand)
    {
        printf("error#1: code command answer\n");
        return FALSE;
    }
    for (i = 0; i < 7; i++)
        if (headAnswer.answer.bytes.emptyBytes[i] != 0x00){
            printf("error#2: empty bytes answer\n");
            return FALSE;
        }
    crc32 = CRC32(headAnswer.answer.value, 10);
    if(!CheckCRC32(crc32, headAnswer.answer.bytes.Crc32)){
        printf("error#3: CRC32 answer\n");
        return FALSE;
    }
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




/*
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

}*/










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












