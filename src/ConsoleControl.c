#include "../inc/ConsoleControl.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#include <inttypes.h>

#include <winbase.h>
#include <windows.h>

#include <stdbool.h>




void InitConsoleControl()
{
    InputCommand();

}

uint8_t InputCommand()
{
    uint8_t command = 0;
/*
    scanf("%d", &command);
    printf("Command input - %X\n", command);

    return command;
    */
    printf("Input command\n");
    printf("1 - download driver for PLIS1 in MM935\n");
    //char command = getchar();
    scanf("%d", &command);
    printf("Command - %X\n", command);
    //if (command == '#')
    return command;
}

