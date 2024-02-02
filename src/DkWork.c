#include "../inc/DkWork.h"
#include "../inc/PortMenu.h"
#include "../inc/ConsoleControl.h"
#include "../inc/LoaderControl.h"
#include "../inc/WorkWithFile.h"


uint32_t sizeFilePlis;
uint32_t addrFilePlisInDk;

int switcherPlis = 0;
uint32_t commandPlis;
//uint32_t currentPlisAnswer = 0;


/*
*   Main of this file:
*   Загрузчик ПО ПЛИС в ДК
*/
BOOL LoadInDkPlisFirmware()
{
    int i;
    uint8_t command[14];
    uint32_t currentPlis;

    if (!CheckCurrentPlis(&currentPlis))
        return FALSE;
    if (!SetSettingsComPort(COM_PORT_4, CBR_115200))
        return FALSE;

    //currentPlisAnswer = currentPlis;
    FormCommandDkDriverUpdate(command, currentPlis);

    if (!TxRx_DataControl(command, 14, READ_TIME_COMMAND))
        return FALSE;
    if (!TransmitDataFile(currentPlis))
        return FALSE;
    return TRUE;
}

/* Получаем значение:
*   Номер ПЛИС
*   Адрес по которому грузить прошивку
*   Размер прошивки
*/
BOOL CheckCurrentPlis(uint32_t *currentPlis)
{
    if (commandPlis == 1)
    {
        *currentPlis = PLIS1;
        addrFilePlisInDk = 0;
        sizeFilePlis = SIZE_PLIS_RUSSIAN;
        //printf("\ncurrent PLIS = %d\n", *currentPlis);
        return TRUE;
    }
    if (commandPlis == 2)
    {
        *currentPlis = PLIS2;
        addrFilePlisInDk = SIZE_PLIS_RUSSIAN;
        sizeFilePlis = SIZE_PLIS_RUSSIAN;
        //printf("current PLIS = %d", *currentPlis);
        return TRUE;
    }
    if (commandPlis == 3)
    {
        *currentPlis = PLIS_CYCLONE;
        addrFilePlisInDk = SIZE_PLIS_RUSSIAN * 2;
        sizeFilePlis = SIZE_PLIS_CYCLONE;
        return TRUE;
    }
    if (commandPlis == 4)
    {
        *currentPlis = PLIS3;
        addrFilePlisInDk = SIZE_PLIS_RUSSIAN * 2;
        sizeFilePlis = SIZE_PLIS_RUSSIAN;
        return TRUE;
    }
    if (commandPlis == 5)
    {
        *currentPlis = PLIS4;
        addrFilePlisInDk = SIZE_PLIS_RUSSIAN * 2 + SIZE_PLIS_RUSSIAN;
        sizeFilePlis = SIZE_PLIS_RUSSIAN;
        return TRUE;
    }

    if (commandPlis == 6)   // ÄËß ÂÑÅÕ ÂÎÇÌÎÆÍÛÕ ÏËÈÑ ÄËß ÄÊ
    {
        *currentPlis = ALL_SET1;
        addrFilePlisInDk = 0;
        sizeFilePlis = SIZE_PLIS_RUSSIAN;
        return TRUE;
    }
    if (commandPlis == 7)   // ÄËß ÂÑÅÕ ÂÎÇÌÎÆÍÛÕ ÏËÈÑ ÄËß ÄÊ
    {
        *currentPlis = ALL_SET2;
        addrFilePlisInDk = 0;
        sizeFilePlis = SIZE_PLIS_RUSSIAN;
        return TRUE;
    }

    printf("error current PLIS");
    return FALSE;
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
                printf("First cycle error");
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
