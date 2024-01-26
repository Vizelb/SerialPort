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

uint8_t dataBuff[SIZE_DATA_TO_PC + 4];

extern const uint32_t crc32Table[256];

void InitPortMenu()
{
    int i, j;

    printf("Start\n");
    BREAK_LINE();

    // #1
    printf("Try to open port\n");
    if (!init_com_port(COM_PORT_4))
    {
        printf("Error1\n");
        return;
    }
    BREAK_LINE();

    // #2
    printf("Try to configurate port\n");
    if(!config_com_port(CBR_115200))
    {
        printf("Error2\n");
        return;
    }
    BREAK_LINE();

    // #3
    printf("Start Work\n");
    commandPlis = ConsoleCommandDkDriverUpdate();
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

void ReadBufferShum()
{
    int i, j;
    int counterPackage = 0;

    uint8_t command[10];
    uint32_t dataWrote;
    uint32_t crc32;
    uint32_t crcINT2 = 0xFFFFFFFF;

    char *buffer;

    for (i = 0; i < 10; i++)
        command[i] = 0x55;

    FILE *file;

    file = fopen("D:/Danya/Libary/C/newFileShum.bin", "wb");

    printf("Start\n");
    if (!init_com_port(COM_PORT_9))
    {
        printf("Error Init Port\n");
        return;
    }
    if(!config_com_port(CBR_9600))
    {
        printf("Error Config Port\n");
        return;
    }

    printf("Start Work\n");

    while (1)
    {
        if(!read_data_array_com_port_UPGRADE(dataBuff, SIZE_DATA_TO_PC))
            printf("Error read");

        /*crc32 = CRC32(dataBuff, SIZE_DATA_TO_PC);
        if(!CheckCRC32(crc32, &dataBuff[SIZE_DATA_TO_PC]))
        {
            printf("\n\n ERROR KS");
            //while(1){};
        }*/

        //dataBuff[0] = 0x55;


        printf(" - %d\n", dataBuff[0]);
        dataWrote = fwrite(dataBuff, sizeof(uint8_t), SIZE_DATA_TO_PC, file);
        counterPackage++;
        printf("\nTotal Bytes Receive = %d, = 0x%X ", counterPackage*SIZE_DATA_TO_PC, counterPackage*SIZE_DATA_TO_PC);
        printf(" - %d\n", dataWrote);

        crcINT2 = (crcINT2 >> 8) ^ crc32Table[(crcINT2 ^ dataBuff[0]) & 0xFF];
        printf("crcINT2 = %8.X\n", crcINT2);
        if (counterPackage == 1250000)
        {
            crcINT2 = crcINT2 ^ 0xFFFFFFFF;
            printf("crcINT2 = %8.X\n", crcINT2);
            counterPackage = 0;//break;
            crcINT2 = 0xFFFFFFFF;
            break;
        }
    }

    //close_com_port();
    printf("\nEnd\n");

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


// 2 variant - работает
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

    //dcbSerialParams.BaudRate = CBR_115200;    //CBR_115200;
    dcbSerialParams.BaudRate = baudRate;        //CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.Parity = NOPARITY;
    dcbSerialParams.StopBits = ONESTOPBIT;
    //dcbSerialParams.StopBits = TWOSTOPBITS;

    if (SetCommState(hComm, &dcbSerialParams) == FALSE)
    {
        printf("Error configurate port\n");
        return FALSE;
    }

    printf("Successfully configurate port\n");
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

BOOL read_data_array_com_port_UPGRADE(uint8_t *answerMk, uint16_t dataSize)
{

    int i = 0;

    COMSTAT comstat;

    DWORD dwBytesRead = 0;          // кол-во прочитанных байтов
    DWORD btr = dataSize;
    DWORD temp, mask, signal;
    //overlapped.hEvent = CreateEvent (NULL, true, true, NULL);

    //sync.hEvent = CreateEvent (NULL, TRUE, FALSE, NULL);
    //if (SetCommMask(hComm, EV_RXCHAR))
    //{
        //WaitCommEvent(hComm, &state_s, &sync);
        //wait_s = WaitForSingleObject(sync.hEvent, READ_TIME_DATA);
        //if (wait_s == WAIT_OBJECT_0)
        //{
            ReadFile(hComm, answerMk, btr, &temp, /*&sync*/NULL);
            /*wait_s = WaitForSingleObject(sync.hEvent, READ_TIME_DATA);
            if (wait_s == WAIT_OBJECT_0)
            {
                if (GetOverlappedResult(hComm, &sync, &temp, FALSE))
                {
                    result = temp;
                } else return FALSE;
            } else return FALSE;*/
            //printf("\n\n");
            for (i = 0; i < temp; i++)
            {
                printf("%X ", answerMk[i]);
            }
            //printf("\n");
        //}
        //else return FALSE;
    //}
    //else return FALSE;
    //CloseHandle(sync.hEvent);
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

void CancelFunctiontIoEx()
{
    CancelIoEx(hComm, NULL);
}

void close_com_port()
{
    CloseHandle(hComm);
}









// Старая ф-ция, не используется
/*BOOL read_com_port()
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

