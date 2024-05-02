#include "../inc/ConsoleControl.h"
#include "../inc/PortMenu.h"
#include "../inc/LoaderControl.h"
#include "../inc/DkWork.h"

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

extern uint32_t commandPlis;

uint8_t plisNumTemp, currentBlockTemp, rpzuNumberTemp, plisTypeTemp, rpzuFileNumberTemp;

void InitConsoleControl()
{
    //InputCommand();

}

int DuConsoleCommand(void)
{
    int command;

    printf("Input command\n");
    printf("1 - Create own commands to download driver for PLIS throw MM871 in MM935/BA435/MM937\n");
    printf("2 - Commands to download driver for PLIS throw MM871 in MM935/BA435/MM937\n");
    printf("3 - update Dk (D6) driver\n");
    printf("4 - Commands to download driver for PLIS in MM935/BA435/MM937\n");
    printf("9 - End of Work\n");
    scanf("%d", &command);
    printf("Command - %X\n", command);

    if (command == 1)
        FormCommandPoPlisDuUpdate();
    if (command == 2)
        CommandPoPlisDuProtocol();
    if (command == 3)
        commandPlis = ConsoleCommandDkDriverUpdate();
    if (command == 4)
        CommandPoPlisDuProtocol();

    return command;
}

void FormCommandPoPlisDuUpdate()
{
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
}

void CommandPoPlisDuProtocol()
{
    int command = 0;

    printf("\nInput currentBlock\n 1 - Block A;\n 2 - Block B;\n 3 - Block V;\n");
    scanf("%d", &currentBlockTemp);
    currentBlock = currentBlockTemp;
    printf("\n");
    // DESCRIPTION
    if (currentBlock == 1 || currentBlock == 2) {
        printf("1. PC - block A, B\n");
        printf("Description: nCE_EPROM_1, File in RPZU = 0, type of PLIS = 0, Number PLIS = 0\n");

        printf("2. LLIC - block A, B\n");
        printf("Description: nCE_EPROM_1, File in RPZU = 1, type of PLIS = 0, Number PLIS = 1\n");

        printf("3. PPC - block A, B\n");
        printf("Description: nCE_EPROM_1, File in RPZU = 2, type of PLIS = 0, Number PLIS = 0\n");

        printf("4. PLLIC - block A, B\n");
        printf("Description: nCE_EPROM_1, File in RPZU = 3, type of PLIS = 0, Number PLIS = 1\n");
    }

    if (currentBlock == 3) {
        printf("5. PCU - block V\n");
        printf("Description: nCE_EPROM_1, File in RPZU = 0, type of PLIS = 0, Number PLIS = 0\n");

        printf("6. LLICU - block V\n");
        printf("Description: nCE_EPROM_1, File in RPZU = 1, type of PLIS = 0, Number PLIS = 1\n");
    }

    if (currentBlock == 1 || currentBlock == 2) {
        printf("7. PK - block A, B\n");
        printf("Description: nCE_EPROM_1, File in RPZU = 4, type of PLIS = 0, Number PLIS = 0\n");

        printf("8. LLIK - block A, B\n");
        printf("Description: nCE_EPROM_1, File in RPZU = 5, type of PLIS = 0, Number PLIS = 1\n");
    }

    if (currentBlock == 1 || currentBlock == 2 || currentBlock == 3) {
        printf("9. Transit - block A, B, V \n");
        printf("Description: nCE_EPROM_2, File in RPZU = 0, type of PLIS = 0, Number PLIS = 0\n");

        printf("10. Transit - block A, B, V\n");
        printf("Description: nCE_EPROM_2, File in RPZU = 1, type of PLIS = 0, Number PLIS = 1\n");

        printf("11. KPA - block A, B, V\n");
        printf("Description: nCE_EPROM_2, File in RPZU = 2, type of PLIS = 0, Number PLIS = 0\n");

        printf("12. KPA - block A, B, V\n");
        printf("Description: nCE_EPROM_2, File in RPZU = 3, type of PLIS = 0, Number PLIS = 1\n");
    }

    if (currentBlock == 1 || currentBlock == 2) {
        printf("13. Interface - block A, B\n");
        printf("Description: nCE_EPROM_2, File in RPZU = 4, type of PLIS = 1, Number PLIS = 2\n");
    }

    if (currentBlock == 3) {
        printf("14. Interface - block V\n");
        printf("Description: nCE_EPROM_2, File in RPZU = 4, type of PLIS = 0, Number PLIS = 2\n");
    }

    scanf("%d", &command);
    if (command == 1){
        rpzuNumber = 1;
        rpzuFileNumber = 1;
        plisType = 1;
        plisNum = 1;
    }
    else if (command == 2){
        rpzuNumber = 1;
        rpzuFileNumber = 2;
        plisType = 1;
        plisNum = 2;
    }
    else if (command == 3){
        rpzuNumber = 1;
        rpzuFileNumber = 3;
        plisType = 1;
        plisNum = 1;
    }
    else if (command == 4){
        rpzuNumber = 1;
        rpzuFileNumber = 4;
        plisType = 1;
        plisNum = 2;
    }
    else if (command == 5){
        rpzuNumber = 1;
        rpzuFileNumber = 1;
        plisType = 1;
        plisNum = 1;
    }
    else if (command == 6){
        rpzuNumber = 1;
        rpzuFileNumber = 2;
        plisType = 1;
        plisNum = 2;
    }
    else if (command == 7){
        rpzuNumber = 1;
        rpzuFileNumber = 5;
        plisType = 1;
        plisNum = 1;
    }
    else if (command == 8){
        rpzuNumber = 1;
        rpzuFileNumber = 6;
        plisType = 1;
        plisNum = 2;
    }
    else if (command == 9){
        rpzuNumber = 2;
        rpzuFileNumber = 1;
        plisType = 1;
        plisNum = 1;
    }
    else if (command == 10){
        rpzuNumber = 2;
        rpzuFileNumber = 2;
        plisType = 1;
        plisNum = 2;
    }
    else if (command == 11){
        rpzuNumber = 2;
        rpzuFileNumber = 3;
        plisType = 1;
        plisNum = 1;
    }
    else if (command == 12){
        rpzuNumber = 2;
        rpzuFileNumber = 4;
        plisType = 1;
        plisNum = 2;
    }
    else if (command == 13){
        rpzuNumber = 2;
        rpzuFileNumber = 5;
        plisType = 2;
        plisNum = 3;
    }
    else if (command == 14){
        rpzuNumber = 2;
        rpzuFileNumber = 5;
        plisType = 1;
        plisNum = 3;
    }
    printf("Description Result: currentBlock = %d, nCE_EPROM_%d, File in RPZU = %d, type of PLIS = %d, ¹ PLIS = %d\n", currentBlock, rpzuNumber, rpzuFileNumber-1, plisType-1, plisNum-1);
}

int ConsoleCommandDkDriverUpdate(void)
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

    scanf("%d", &command);
    printf("Command - %X\n", command);
    return command;
}


int JustInputConsoleCommand(void)
{
    int command = 0;
    printf("Input command <5> to continue\n");
    scanf("%d", &command);
    return command;
}






