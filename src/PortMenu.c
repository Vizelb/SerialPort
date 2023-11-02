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

// 1 variant
struct termios serial_settings;
int serial_port;

unsigned char bufrd[255];

// 2 variant
#include <windows.h>

HANDLE hComm;

OVERLAPPED overlapped;

void InitPortMenu()
{
    int i, j;
    char temp = 'A';
    printf("Start\n");
    printf("Let's go!!!\n");

    //OpenSerialPort();
    //ConfigurateSerialPort();


    if (!init_com_port())
    {
        printf("Error1\n");
        return;
    }
    printf("Next1\n");
    if(!config_com_port())
    {
        printf("Error2\n");
        return;
    }
    while (1)
    {
        TransmissionCycle();
    }

    printf("Prepare To Close Port\n");
    close_com_port();
    printf("End\n");

}


void TransmissionCycle()
{


    //sleep(1);
        printf("Prepare To Send\n");
    //for (i = 0; i < 10; i++)
    //{
        //Sleep(1000);
        //printf("pospal");
        if(!send_char(0xFA))
        {
            printf("Error send_char 3\n");
            return;
    //}

        }

        printf("Prepare To Read\n");
        //sleep(1);
        //sleep(1);
    //SetCommMask(HANDLE hComm, DWORD dwEvtMask);

        SetCommMask(hComm, EV_RXCHAR);

            if(!read_com_port())
            {
                printf("Error read_com_port 4\n");
                return;
            }

        //sleep(1);

}


// 2 variant - работает
BOOL init_com_port()
{
    hComm = CreateFile("COM4", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
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

    if (SetCommState(hComm, &dcbSerialParams) == FALSE)
        return FALSE;

    return TRUE;
}




BOOL send_char(uint8_t c)
{
    int i;
    DWORD dwBytesWritten;
    //char msg[] = {c, '\0'};
    //uint8_t data[] = {c, '\0'};
    uint8_t data[12];// = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,'\0'};
    for (i = 0; i < 12; i++)
    {
        data[i] = 0x01;
    }

    if (WriteFile(hComm,            // дескриптор устр
                  data,              // указатель на буфер
                  //sizeof(msg),      // длина буфера
                  sizeof(data),      // длина буфера
                  &dwBytesWritten,  // кол-во записанных байтов
                  NULL))            // overlapped атрибут
        return TRUE;
    else return FALSE;
}

BOOL read_com_port()
{
    COMSTAT comstat;

    DWORD dwBytesRead = 0;          // кол-во прочитанных байтов
    DWORD btr, temp, mask, signal;
    overlapped.hEvent = CreateEvent (NULL, true, true, NULL);
    //char chRet = '\0';
    uint8_t chRet = '\0';
    int i = 0;
    WaitCommEvent(hComm, &mask, &overlapped);
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

                        /*while (i < 12)
                        {
                            if (ReadFile(hComm,
                            &chRet,
                            sizeof(chRet),
                            &dwBytesRead,
                            NULL))
                            {
                                if (i == 0 && (chRet == 0))
                                    return FALSE;
                                fprintf(stdout, "%X", chRet);
                                i++;
                            }

                        }*/
    fprintf(stdout, "\n");

                    }
                }
            }
        }
    return TRUE;

    /*while (i < 12)
    {
        if (ReadFile(hComm,
                     &chRet,
                     sizeof(chRet),
                     &dwBytesRead,
                     NULL))
        {
            if (i == 0 && (chRet == 0))
                return;
            fprintf(stdout, "%X", chRet);
            i++;
        }

    }
    fprintf(stdout, "\n");*/
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

