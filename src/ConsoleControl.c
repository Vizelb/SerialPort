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

uint8_t InputCommand();


void InitConsoleControl()
{
    InputCommand();

}

uint8_t InputCommand()
{
    /*uint8_t command = 0;

    scanf("%d", &command);
    printf("Command input - %X\n", command);

    return command;
    */
    printf("Input command\n");
    char command = getchar();
    printf("Command - %c\n", command);
    //if (command == '#')
    return command;
}

