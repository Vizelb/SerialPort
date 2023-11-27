#include "../inc/LoaderControl.h"
#include "../inc/PortMenu.h"

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



uint32_t sizeFilePlis;
uint32_t addrFilePlisInDk;

BOOL InitLoaderControl(HANDLE hComm, uint32_t currentPlis)
{
    int i;
    uint8_t command[14];

    FormCommand(command, currentPlis);


    printf("Prepare To Send Data Command\n");
    if(!send_char(0xFA))
    //if(!send_array(command, 12))
    {
        printf("Error send char Command 3\n");
        return FALSE;
    }
    printf("Command Send\n");
    //BREAK_LINE();
    //sleep(1);
    //SetCommMask(hComm, EV_RXCHAR);

    CancelIoEx(hComm, NULL);

    printf("Prepare To Read\n");
    if(!read_command_com_port())
    {
        printf("Error read com port Command\n");
        return FALSE;
    }
    CancelIoEx(hComm, NULL);
    if(!CheckAnswerCommand())
    {
        printf("Error Control Command\n");
        return FALSE;
    }
    return TRUE;
}

void FormCommand(uint8_t *command, uint32_t currentPlis)
{
    int i;
    uint32_t crc32;
    struct CommandToMk headCommand;

    headCommand.command.bytes.code = KKU;
    headCommand.command.bytes.plisNumber = currentPlis;

    if (currentPlis != PLIS_CYCLONE)
    {
        headCommand.command.bytes.sizeFile[0] = SIZE_PLIS_RUSSIAN;
        headCommand.command.bytes.sizeFile[1] = SIZE_PLIS_RUSSIAN << 8;
        headCommand.command.bytes.sizeFile[2] = SIZE_PLIS_RUSSIAN << 16;
        headCommand.command.bytes.sizeFile[3] = SIZE_PLIS_RUSSIAN << 24;
    }
    else
    {
        headCommand.command.bytes.sizeFile[0] = SIZE_PLIS_CYCLONE;
        headCommand.command.bytes.sizeFile[1] = SIZE_PLIS_CYCLONE << 8;
        headCommand.command.bytes.sizeFile[2] = SIZE_PLIS_CYCLONE << 16;
        headCommand.command.bytes.sizeFile[3] = SIZE_PLIS_CYCLONE << 24;
    }

    headCommand.command.bytes.addrDk[0] = addrFilePlisInDk;
    headCommand.command.bytes.addrDk[1] = addrFilePlisInDk << 8;
    headCommand.command.bytes.addrDk[2] = addrFilePlisInDk << 16;
    headCommand.command.bytes.addrDk[3] = addrFilePlisInDk << 24;

    crc32 = CRC32(headCommand, 10);
    headCommand.command.bytes.Crc32[0] = crc32;
    headCommand.command.bytes.Crc32[1] = crc32 << 8;
    headCommand.command.bytes.Crc32[2] = crc32 << 16;
    headCommand.command.bytes.Crc32[3] = crc32 << 24;


    //for (i = 0; i < 14; i++)
        command = headCommand.command.value;
}

BOOL CheckCurrentPlis(uint32_t command, uint32_t *currentPlis)
{
    if (command == 1)
    {
        *currentPlis = PLIS1;
        addrFilePlisInDk = 0;
        return TRUE;
    }
    if (command == 2)
    {
        *currentPlis = PLIS2;
        addrFilePlisInDk = SIZE_PLIS_RUSSIAN;
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
    return FALSE;
}






