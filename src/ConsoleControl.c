#include "../inc/ConsoleControl.h"
#include "../inc/PortMenu.h"
#include "../inc/LoaderControl.h"

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

extern uint8_t rpzuNumber;
extern uint8_t plisNum;
extern uint8_t plisType;
extern uint8_t rpzuFileNumber;
extern uint8_t currentBlock;

uint8_t plisNumTemp, currentBlockTemp, rpzuNumberTemp, plisTypeTemp, rpzuFileNumberTemp;

void InitConsoleControl()
{
    InputCommand();

}

int InputCommand(void)
{
    int command = 0;
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

int DuConsoleCommand(void)
{
    int command;


    printf("Input command\n");
    printf("1 - download driver for PLIS in MM935/BA435/MM937\n");
    scanf("%d", &command);
    printf("Command - %X\n", command);

    if (command == 9)
        return command;

    printf("\nInput currentBlock\n 1 - Block A;\n 2 - Block B;\n 3 - Block V;\n");
    scanf("%d", &currentBlockTemp);
    printf("Command currentBlockTemp - %X\n", currentBlockTemp);

    printf("\nInput rpzuNumber - 1 or 2\n");
    scanf("%d", &rpzuNumber);
    printf("Command rpzuNumber - %X\n", rpzuNumber);


    printf("\nInput plisNumber - 1, 2, 3, 4\n");
    scanf("%d", &plisNumTemp);
    //plisNum = plisNumber;
    //printf("Command plisNum - %X\n", plisNum);
    printf("Command plisNumTemp - %X \n", plisNumTemp);

    printf("\nInput plisType - 1 or 2\n");
    scanf("%d", &plisType);
    printf("Command plisType - %X \n", plisType);

    printf("\nInput rpzuFileNumber - 1, 2, 3, 4, 5, 6 \n");
    scanf("%d", &rpzuFileNumber);
    printf("Command rpzuFileNumber - %X \n", rpzuFileNumber);

    /*printf("Input plisNumTemp - 1, 2, 3, 4\n");
    scanf("%d", &plisNum);
    printf("Command plisNumTemp - %X \n", plisNum);*/

    plisNum = plisNumTemp;
    printf("\nCommand plisNumber - %X\n", plisNum);

    currentBlock = currentBlockTemp;
    printf("Command currentBlock - %X\n", currentBlock);
/*
    rpzuNumber = rpzuNumberTemp;
    printf("Command rpzuNumber - %X\n", rpzuNumber);

    plisType = plisTypeTemp;
    printf("Command plisType - %X\n", plisType);

    rpzuFileNumber = rpzuFileNumberTemp;
    printf("Command rpzuFileNumber - %X\n", rpzuFileNumber);*/

    return command;
}



