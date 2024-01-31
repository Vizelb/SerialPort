#include "../inc/PortMenu.h"
#include "../inc/ConsoleControl.h"
#include "../inc/LoaderControl.h"
#include "../inc/CRC32.h"


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
#include <windows.h>


HANDLE hComm;
OVERLAPPED sync = {0};

unsigned long wait_s = 0;
unsigned long state_s = 0;



// Ф-ция отправки данных по порту
// data - данный (байты), которые отправляются
// dataSize - кол-во байт на отпрвку (размер массива данных)
BOOL SendData(uint8_t *data, uint16_t dataSize)
{
    DWORD dwBytesWritten;
    DWORD feedBack;

    if (WriteFile(hComm,            // дескриптор устр
                  data,             // указатель на буфер
                  dataSize,         //dataSize,//(DWORD)sizeof(data),      // длина буфера
                  &dwBytesWritten,  // кол-во записанных байтов
                  NULL)             // overlapped атрибут
            || feedBack != (DWORD)sizeof(data))
        return TRUE;
    else {
        printf("Error Send data\n");
        return FALSE;
    }
}

// Чтение Данных из порта
// data - массив данных (байтов), в который производится запись данных
// dataSize - размер читаемых данных (размер массива данных, которое нужно прочитать)
// time_data - кол-во времени ожидаемое на чтение данных
BOOL ReadData(uint8_t *data, uint16_t dataSize, int time_data)
{
    int i = 0;
    DWORD btr = dataSize;
    DWORD temp;

    sync.hEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
    if (SetCommMask(hComm, EV_RXCHAR))
    {
        WaitCommEvent(hComm, &state_s, &sync);
        wait_s = WaitForSingleObject(sync.hEvent, time_data);
        if (wait_s == WAIT_OBJECT_0)
        {
            ReadFile(hComm, data, btr, &temp, /*&sync*/NULL);
            wait_s = WaitForSingleObject(sync.hEvent, time_data);
            if (wait_s == WAIT_OBJECT_0)
            {
                if (GetOverlappedResult(hComm, &sync, &temp, FALSE))
                {
                    printf("result of GetOverlappedResult = %d\n", temp);
                } else return FALSE;
            } else return FALSE;
            //printf("\n\n");
            for (i = 0; i < temp; i++)
            {
                printf("%X ", data[i]);
            }
            //printf("\n");
        }
        else return FALSE;
    }
    else return FALSE;
    CloseHandle(sync.hEvent);
    printf("Successfully read data!\n");
    return TRUE;
}


// Чтение массива данных
// Используется в ReadShum();
BOOL read_data_array_com_port_Shum(uint8_t *data, uint16_t dataSize)
{
    int i = 0;
    DWORD btr = dataSize;
    DWORD temp;

    if(!ReadFile(hComm, data, btr, &temp, NULL))
    {
        printf("Read data error !&$?#\n");
        return FALSE;
    }
    for (i = 0; i < temp; i++)
        printf("%X ", data[i]);
    return TRUE;
}


BOOL SetSettingsComPort(char *com_port, int baudRate)
{
    printf("Try to open the port...\n");
    if (!init_com_port(com_port))
    {
        printf("Port opening error $%!?#\n");
        return FALSE;
    }
    printf("Port is opened!\n");
    printf("Try to configure the port...\n");
    if (!config_com_port(baudRate))
    {
        printf("Port configuration error $%!?#\n");
        return FALSE;
    }
    printf("Port configured!\n");
    return TRUE;
}

// Инициализация Порта
// 1 параметр com_port - выбранный порт
BOOL init_com_port(char *com_port)
{
    hComm = CreateFile(com_port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0 /*FILE_ATTRIBUTE_NORMAL*/, NULL);
    if (hComm == INVALID_HANDLE_VALUE)
        return FALSE;
    return TRUE;
}

// Ф-ция конфигурации порта
// bautRate - скорость работы конфигурируемого порта
BOOL config_com_port(int baudRate)
{
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (GetCommState(hComm, &dcbSerialParams) == FALSE)
        return FALSE;
    dcbSerialParams.BaudRate = baudRate;    //CBR_115200; //CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.Parity = NOPARITY;
    dcbSerialParams.StopBits = ONESTOPBIT;
    if (SetCommState(hComm, &dcbSerialParams) == FALSE)
        return FALSE;
    return TRUE;
}

// Отменяет все ожидающие операции ввода-вывода
BOOL CancelFunctiontIoEx()
{
    if (!CancelIoEx(hComm, NULL))
        return FALSE;
    if (!CancelIo(hComm))
        return FALSE;
    return TRUE;
}

BOOL close_com_port()
{
    if (!CloseHandle(hComm))
    {
        printf("Port closing error $!#@:&\n");
        return FALSE;
    }
    printf("Port successfully closed!\n");
    return TRUE;
}




















