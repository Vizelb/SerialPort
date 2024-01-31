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

#define BREAK_LINE()    printf("//////// ////////\n\n")

HANDLE hComm;
OVERLAPPED overlapped;

DWORD dwBytesWrite = 0;

OVERLAPPED sync = {0};
int result = 0;
unsigned long wait_s = 0;
unsigned long read_s = 0;
unsigned long state_s = 0;

uint32_t commandPlis;
uint32_t currentPlis;



extern const uint32_t crc32Table[256];





BOOL send_command(uint8_t *data)
{
    int i;
    DWORD dwBytesWritten;
    DWORD feedBack;
    //char msg[] = {c, '\0'};
    //uint8_t data[] = {c, '\0'};
    /*uint8_t data[14];// = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,'\0'};
    for (i = 0; i < sizeof(data); i++)
    {
        data[i] = command[i];     // открывать файл заранее; здесь передавать размер файла и адрес куда записывать
    }*/

    //PurgeComm(hComm, PURGE_TXABORT | PURGE_TXCLEAR);
    if (WriteFile(hComm,            // дескриптор устр
                  data,              // указатель на буфер
                  14,//(DWORD)sizeof(data),      // длина буфера
                  &dwBytesWritten,  // кол-во записанных байтов
                  NULL)             // overlapped атрибут
            || feedBack != (DWORD)sizeof(data)
       )
    {
        return TRUE;
    }
    else return FALSE;
}


BOOL send_data(uint8_t *dataArray, uint16_t arraySize)
{
    int i;
    DWORD dwBytesWritten;
    DWORD feedBack;
    //printf("Start Send data\n");

    if (WriteFile(hComm,            // дескриптор устр
                  dataArray,              // указатель на буфер
                  arraySize,//arraySize,//(DWORD)sizeof(dataArray),      // длина буфера
                  &dwBytesWritten,  // кол-во записанных байтов
                  NULL)             // overlapped атрибут
            || feedBack != (DWORD)sizeof(dataArray)
       )
    {
        //printf("End Send data\n");
        return TRUE;
    }
    else
    {
        printf("Error Send data\n");
        return FALSE;
    }
    //if (WriteFile())
    //if (TransmitFile(hComm, ))
}


BOOL read_data_array_com_port(uint8_t *answerMk, uint16_t dataSize)
{

    int i = 0;

    COMSTAT comstat;

    DWORD dwBytesRead = 0;          // кол-во прочитанных байтов
    DWORD btr = dataSize;
    DWORD temp, mask, signal;
    //overlapped.hEvent = CreateEvent (NULL, true, true, NULL);

    sync.hEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
    if (SetCommMask(hComm, EV_RXCHAR))
    {
        WaitCommEvent(hComm, &state_s, &sync);
        wait_s = WaitForSingleObject(sync.hEvent, READ_TIME_DATA);
        if (wait_s == WAIT_OBJECT_0)
        {
            ReadFile(hComm, answerMk, btr, &temp, /*&sync*/NULL);
            wait_s = WaitForSingleObject(sync.hEvent, READ_TIME_DATA);
            if (wait_s == WAIT_OBJECT_0)
            {
                if (GetOverlappedResult(hComm, &sync, &temp, FALSE))
                {
                    result = temp;
                } else return FALSE;
            } else return FALSE;
            //printf("\n\n");
            for (i = 0; i < temp; i++)
            {
                printf("%X ", answerMk[i]);
            }
            //printf("\n");
        }
        else return FALSE;
    }
    else return FALSE;
    CloseHandle(sync.hEvent);
    return TRUE;
}


BOOL read_command_com_port(uint8_t *data, uint16_t dataSize)
{
    uint8_t chRet = '\0';
    int i = 0;

    COMSTAT comstat;

    DWORD dwBytesRead = 0;          // кол-во прочитанных байтов
    DWORD btr = dataSize;
    DWORD temp, mask, signal;
    //overlapped.hEvent = CreateEvent (NULL, true, true, NULL);

    sync.hEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
    if (SetCommMask(hComm, EV_RXCHAR))
    {
        WaitCommEvent(hComm, &state_s, &sync);
        wait_s = WaitForSingleObject(sync.hEvent, READ_TIME_COMMAND);
        if (wait_s == WAIT_OBJECT_0)
        {
            ReadFile(hComm, data, btr, &temp, &sync);
            wait_s = WaitForSingleObject(sync.hEvent, READ_TIME_COMMAND);
            if (wait_s == WAIT_OBJECT_0)
                if (GetOverlappedResult(hComm, &sync, &temp, FALSE))
                    result = temp;
            for (i = 0; i < temp; i++)
            {
                printf("%X ", data[i]);
                //printf(";");
            }
            //printf("\n%dx\n", &data);
            //fprintf(stdout, "\n");
            printf("\n");
        }
        else return FALSE;
    }
    else return FALSE;
    //CloseHandle(sync.hEvent);
    return TRUE;
}

// Чтение массива данных
// Используется в ReadShum();
void read_data_array_com_port_Shum(uint8_t *answerMk, uint16_t dataSize)
{
    int i = 0;
    DWORD btr = dataSize;
    DWORD temp;

    ReadFile(hComm, answerMk, btr, &temp, NULL);
    for (i = 0; i < temp; i++)
        printf("%X ", answerMk[i]);
}

// Инициализация Порта
// 1 параметр com_port - выбранный порт
BOOL init_com_port(char *com_port)
{
    hComm = CreateFile(com_port/*SERIAL_PORT*//*"COM9"*/, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL); // РАБОТАЕТ НО КРИВО
    //hComm = CreateFile("COM4", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    //hComm = CreateFile(/*SERIAL_PORT*/"COM4", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    if (hComm == INVALID_HANDLE_VALUE)
    {
        printf("Error opening serial port\n");
        return FALSE;
    }
    printf("Port is open\n");
    return TRUE;
}

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
    {
        printf("Error configurate port\n");
        return FALSE;
    }

    printf("Successfully configurate port\n");
    return TRUE;
}

void CancelFunctiontIoEx()
{
    CancelIoEx(hComm, NULL);
}

void close_com_port()
{
    CloseHandle(hComm);
}




















