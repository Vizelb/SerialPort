#include "../inc/ConsoleControl.h"
#include "../inc/PortMenu.h"

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

//#define PR_TEXT(char text)     printf("%d\n", text)


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
    printf("2 - download driver for PLIS2 in MM935\n");
    printf("3 - download driver for PLIS3 Cyclone in BA435\n");
    printf("4 - download driver for PLIS3 in MM937\n");
    printf("5 - download driver for PLIS4 in MM937\n");

    printf("6 - download 3 driver for PLIS1 + PLIS2 + PLIS_CYCLONE \n");
    printf("7 - download 3 driver for PLIS1 + PLIS2 + PLIS3 + PLIS4 \n");

    //char command = getchar();
    scanf("%d", &command);
    printf("Command - %X\n", command);
    //if (command == '#')
    return command;
}

int testCommand(void)
{
    int command;
    printf("Input command\n");
    scanf("%d", &command);
    printf("Command - %X\n", command);
    return command;
}



