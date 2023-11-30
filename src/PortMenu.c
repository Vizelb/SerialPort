#include "../inc/PortMenu.h"
#include "../inc/ConsoleControl.h"
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

// 1 variant
/*struct termios serial_settings;
int serial_port;
*/

// 2 variant
#include <windows.h>

//unsigned char bufrd[255];
//unsigned char answerCommand[12];

#define BREAK_LINE()    printf("//////// ////////\n\n")

HANDLE hComm;
OVERLAPPED overlapped;


OVERLAPPED sync = {0};
int result = 0;
unsigned long wait_s = 0;
unsigned long read_s = 0;
unsigned long state_s = 0;

uint32_t commandPlis;
uint32_t currentPlis;

void InitPortMenu()
{
    int i, j;

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
    printf("Start Work\n");
    commandPlis = InputCommand();
    if(CheckCurrentPlis(commandPlis, &currentPlis))
    {
        if (!StartLoadingFile(hComm, currentPlis))
        {
            printf("Che za error?");
            return;
        }
    }
    else printf("ERROR COMMAND");

    // #4
    BREAK_LINE();
    printf("Prepare To Close Port\n");
    close_com_port();
    printf("End\n");
    BREAK_LINE();

}


BOOL WorkCycle()
{
    /*int i;
    uint8_t command[14];

    for (i = 0; i < 12; i++)
    {
        command[i] = i;
    }
    //sleep(1);
    //while (1)
    //{
        printf("Prepare To Send Data Command\n");
        if(!send_command(command))
        //if(!send_array(command, 12))
        {
            printf("Error send char Command 3\n");
            return FALSE;
        }
        printf("Command Send\n");
        //BREAK_LINE();
        //sleep(1);
        //SetCommMask(hComm, EV_RXCHAR);

        CancelIoEx(hComm, NULL);

        printf("Prepare To Read\n");
        if(!read_command_com_port(command))
        {
            printf("Error read com port Command\n");
            return FALSE;
        }
        CancelIoEx(hComm, NULL);
        if(!CheckAnswerCommand())
        {
            printf("Error Control Command\n");
            return FALSE;
        }
    return TRUE;*/
}


// 2 variant - ��������
BOOL init_com_port()
{
    hComm = CreateFile("COM4", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL); // �������� �� �����
    //hComm = CreateFile("COM4", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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

    dcbSerialParams.BaudRate = CBR_115200; //CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.Parity = NOPARITY;
    dcbSerialParams.StopBits = ONESTOPBIT;
    //dcbSerialParams.StopBits = TWOSTOPBITS;

    if (SetCommState(hComm, &dcbSerialParams) == FALSE)
        return FALSE;

    return TRUE;
}




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
        data[i] = command[i];     // ��������� ���� �������; ����� ���������� ������ ����� � ����� ���� ����������
    }*/

    //PurgeComm(hComm, PURGE_TXABORT | PURGE_TXCLEAR);
    if (WriteFile(hComm,            // ���������� ����
                  data,              // ��������� �� �����
                  14,//(DWORD)sizeof(data),      // ����� ������
                  &dwBytesWritten,  // ���-�� ���������� ������
                  NULL)             // overlapped �������
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

    if (WriteFile(hComm,            // ���������� ����
                  dataArray,              // ��������� �� �����
                  arraySize,//(DWORD)sizeof(dataArray),      // ����� ������
                  &dwBytesWritten,  // ���-�� ���������� ������
                  NULL)             // overlapped �������
        || feedBack != (DWORD)sizeof(dataArray)
        )
        {
        return TRUE;
        }
    else return FALSE;
    //if (WriteFile())
    //if (TransmitFile(hComm, ))
}


BOOL read_data_array_com_port(uint8_t *answerMk)
{

    int i = 0;

    COMSTAT comstat;

    DWORD dwBytesRead = 0;          // ���-�� ����������� ������
    DWORD btr = 14;
    DWORD temp, mask, signal;
    //overlapped.hEvent = CreateEvent (NULL, true, true, NULL);

    sync.hEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
    if (SetCommMask(hComm, EV_RXCHAR))
    {
        WaitCommEvent(hComm, &state_s, &sync);
        wait_s = WaitForSingleObject(sync.hEvent, READ_TIME_DATA);
        if (wait_s == WAIT_OBJECT_0)
        {
            ReadFile(hComm, answerMk, btr, &temp, &sync);
            wait_s = WaitForSingleObject(sync.hEvent, READ_TIME_DATA);
            if (wait_s == WAIT_OBJECT_0)
            {
                if (GetOverlappedResult(hComm, &sync, &temp, FALSE))
                {
                    result = temp;
                } else return FALSE;
            } else return FALSE;
            printf("\n\n");
            for (i = 0; i < temp; i++)
            {
                printf("%X, ", answerMk[i]);
            }
            printf("\n\n");
        } else return FALSE;
    } else return FALSE;
    //CloseHandle(sync.hEvent);
    return TRUE;
}

BOOL read_command_com_port(uint8_t *data)
{
    uint8_t chRet = '\0';
    int i = 0;

    COMSTAT comstat;

    DWORD dwBytesRead = 0;          // ���-�� ����������� ������
    DWORD btr = 14;
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
                printf("%X, ", data[i]);
                //printf(";");
            }
            //printf("\n%dx\n", &data);
            //fprintf(stdout, "\n");
            printf("\n");
        } else return FALSE;
    } else return FALSE;
    //CloseHandle(sync.hEvent);
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









// ������ �-���, �� ������������
/*BOOL read_com_port()
{
    uint8_t chRet = '\0';
    int i = 0;

    COMSTAT comstat;

    DWORD dwBytesRead = 0;          // ���-�� ����������� ������
    DWORD btr = 12;
    DWORD temp, mask, signal;
    //overlapped.hEvent = CreateEvent (NULL, true, true, NULL);

    sync.hEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
    if (SetCommMask(hComm, EV_RXCHAR))
    {
        WaitCommEvent(hComm, &state_s, &sync);
        wait_s = WaitForSingleObject(sync.hEvent, READ_TIME_COMMAND);
        if (wait_s == WAIT_OBJECT_0)
        {
            ReadFile(hComm, bufrd, btr, &temp, &sync);
            wait_s = WaitForSingleObject(sync.hEvent, READ_TIME_COMMAND);
            if (wait_s == WAIT_OBJECT_0)
                if (GetOverlappedResult(hComm, &sync, &temp, FALSE))
                    result = temp;
            for (i = 0; i < temp; i++)
            {
                printf("%X, ", bufrd[i]);
                //printf(";");
            }
            //printf("\n%dx\n", &bufrd);
            //fprintf(stdout, "\n");
            printf("\n");
        }
    }
    //CloseHandle(sync.hEvent);
    return;*/


    /*signal = WaitForSingleObject(overlapped.hEvent, INFINITE);

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
*/
//}











/*void OpenSerialPort()
{
    // 1 variant - ��������
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

