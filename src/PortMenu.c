#include "../inc/PortMenu.h"
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

// 1 variant
/*struct termios serial_settings;
int serial_port;
*/

// 2 variant
#include <windows.h>

#define BREAK_LINE()    printf("//////// ////////\n\n")

unsigned char bufrd[255];

HANDLE hComm;
OVERLAPPED overlapped;

const int READ_TIME = 100;
OVERLAPPED sync = {0};
int result = 0;
unsigned long wait_s = 0;
unsigned long read_s = 0;
unsigned long state_s = 0;

void InitPortMenu()
{
    int i, j;
    //char temp = 'A';
    printf("Start\n");
    BREAK_LINE();

    // #1
    printf("Try to open port\n");
      if (!init_com_port())
    {
        printf("Error1\n");
        return;
    }
    BREAK_LINE();

    // #2
    printf("Try to configurate port\n");
    if(!config_com_port())
    {
        printf("Error2\n");
        return;
    }
    BREAK_LINE();

    // #3
    printf("Work Cycle\n");
    //while (1)
    //{
        //if (!(InputCommand() == '#')
          //  WorkCycle();
        //break;
    repeatWork();

        //BREAK_LINE();
    //}

    // #4
    printf("Prepare To Close Port\n");
    close_com_port();
    printf("End\n");
    BREAK_LINE();

}

void repeatWork()
{
    if (InputCommand() != '#')
        WorkCycle();
    return;
}


void WorkCycle()
{
    int i;
    uint8_t data[12];
    //char command;

    for (i = 0; i < 12; i++)
    {
        data[i] = i;
    }

    //while (1)
    //{
        printf("Prepare To Send Data\n");
        if(!send_char(0xFA))
        //if(!send_array(data, 12))
        {
            printf("Error send_char 3\n");
            return;
        }
        printf("Command Send\n");
        //BREAK_LINE();

        printf("Prepare To Read\n");
        if(!read_com_port())
        {
            printf("Error read_com_port 4\n");
            return;
        }
        //BREAK_LINE();
        //sleep(2);

    repeatWork();
    return;
    ///////}

    if(!send_char(0xFA))
        //if(!send_array(data, 12))
    {
        printf("Error send_char 3\n");
        return;
    }

    printf("Prepare To Read\n");
    //SetCommMask(HANDLE hComm, DWORD dwEvtMask);
    //SetCommMask(hComm, EV_RXCHAR);

    if(!read_com_port())
    {
        printf("Error read_com_port 4\n");
        return;
    }

    sleep(1);

}


// 2 variant - работает
BOOL init_com_port()
{
    hComm = CreateFile("COM4", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    //hComm = CreateFile(/*SERIAL_PORT*/"COM4", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    if (hComm == INVALID_HANDLE_VALUE)
    {
        printf("Error Error opening serial port\n");
        return FALSE;
    }
    printf("Port is open\n");
    return TRUE;
}

BOOL config_com_port()
{
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (GetCommState(hComm, &dcbSerialParams) == FALSE)
        return FALSE;

    dcbSerialParams.BaudRate = CBR_19200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.Parity = NOPARITY;
    dcbSerialParams.StopBits = ONESTOPBIT;
    //dcbSerialParams.StopBits = TWOSTOPBITS;

    if (SetCommState(hComm, &dcbSerialParams) == FALSE)
        return FALSE;

    return TRUE;
}




BOOL send_char(uint8_t c)
{
    int i;
    DWORD dwBytesWritten = 0;
    DWORD feedBack;
    //char msg[] = {c, '\0'};
    //uint8_t data[] = {c, '\0'};
    uint8_t data[12];// = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,'\0'};
    for (i = 0; i < 12; i++)
    {
        data[i] = 0x01;
    }
    data[0] = 0x79;
    data[1] = 0x75;

    if (WriteFile(hComm,            // дескриптор устр
                  data,              // указатель на буфер
                  (DWORD)sizeof(data),      // длина буфера
                  &dwBytesWritten,  // кол-во записанных байтов
                  NULL)             // overlapped атрибут
        //|| feedBack != (DWORD)sizeof(data)
        )
        {
        return TRUE;
        }
    else return FALSE;
}


BOOL send_array(uint8_t *dataArray, uint16_t arraySize)
{
    int i;
    DWORD dwBytesWritten;
    //char msg[] = {c, '\0'};
    //uint8_t data[] = {c, '\0'};


    if (WriteFile(hComm,            // дескриптор устр
                  dataArray,              // указатель на буфер
                  /*sizeof(dataArray)*/ arraySize,      // длина буфера
                  &dwBytesWritten,  // кол-во записанных байтов
                  NULL))            // overlapped атрибут
        return TRUE;
    else return FALSE;
}



BOOL read_com_port()
{
    uint8_t chRet = '\0';
    int i = 0;

    COMSTAT comstat;

    DWORD dwBytesRead = 0;          // кол-во прочитанных байтов
    DWORD btr = 12;
    DWORD temp, mask, signal;
    //overlapped.hEvent = CreateEvent (NULL, true, true, NULL);

    sync.hEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
    if (SetCommMask(hComm, EV_RXCHAR))
    {
        WaitCommEvent(hComm, &state_s, &sync);
        wait_s = WaitForSingleObject(sync.hEvent, READ_TIME);
        if (wait_s == WAIT_OBJECT_0)
        {
            ReadFile(hComm, bufrd, btr, &temp, &sync);
            wait_s = WaitForSingleObject(sync.hEvent, READ_TIME);
            if (wait_s == WAIT_OBJECT_0)
                if (GetOverlappedResult(hComm, &sync, &temp, FALSE))
                    result = temp;
            for (i = 0; i < temp; i++)
            {
                printf("%X", bufrd[i]);
                //printf(";");
            }
            //printf("\n%dx\n", &bufrd);
            fprintf(stdout, "\n");
        }
    }
    CloseHandle(sync.hEvent);
    return;


    signal = WaitForSingleObject(overlapped.hEvent, INFINITE);

    if (signal == WAIT_OBJECT_0)
    {
            if (GetOverlappedResult(hComm, &overlapped, &temp, true))
            {
                if((mask & EV_RXCHAR) != 0)
                {
                    ClearCommError(hComm, &temp, &comstat);
                    btr = comstat.cbInQue;
                    if(btr)
                    {
                        ReadFile(hComm, bufrd, btr, &temp, &overlapped);
                        for (i = 0; i < temp; i++)
                        {
                            printf("%X", bufrd[i]);
                            printf(";");
                        }
                        printf("\n");
                        fprintf(stdout, "\n");

                    }
                }
            }
        }
    return TRUE;


}



void close_com_port()
{
    CloseHandle(hComm);
}







/*void OpenSerialPort()
{
    // 1 variant - работает
    serial_port = open(SEIAL_PORT, O_RDWR);
    if (serial_port < 0) {
        printf("Error opening serial port\n");
        exit(1);
    }
    else {
        printf("Port is open\n");
    }
}

void ConfigurateSerialPort()
{
    (serial_port, &serial_settings);
    cfsetispeed(&serial_settings, B9600);
    cfsetospeed(&serial_settings, B9600);
    serial_settings.c_cflag &= ~PARENB;
    serial_settings.c_cflag &= ~CSTOPB;
    serial_settings.c_cflag &= ~CSIZE;
    serial_settings.c_cflag |= CS8;
    tcsetattr(serial_port, TCSANOW, &serial_settings);
}

void ReadFromSerialPort()
{
    char buffer[516];
    int n = read(serial_port, buffer, sizeof(buffer));
    if (n < 0) {
        printf("Error reading from serial port \n");
        exit(1);
    }
}*/

