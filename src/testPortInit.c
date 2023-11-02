
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#include <inttypes.h>

#include <winbase.h>
#include <windows.h>



HANDLE hComm;
DWORD iSize;
OVERLAPPED overlapped;
DWORD WINAPI ReadThread (LPVOID);
unsigned char bufrd[255];

initTest()
{

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    GetCommState(hComm, &dcbSerialParams);


    dcbSerialParams.BaudRate = CBR_19200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.Parity = NOPARITY;
    dcbSerialParams.StopBits = ONESTOPBIT;

    SetCommState(hComm, &dcbSerialParams);

    COMMTIMEOUTS cc;
    cc.ReadTotalTimeoutMultiplier = 1000;
    cc.ReadIntervalTimeout = 1000000;
    cc.ReadTotalTimeoutConstant = 100000;

    SetCommTimeouts(hComm, &cc);



    COMSTAT comstat;

    DWORD btr, temp, mask, signal;
    overlapped.hEvent = CreateEvent (NULL, true, true, NULL);

    SetCommMask(hComm, EV_RXCHAR);

    HANDLE reader;
    reader = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);

    while (true)
    {

    }
    return;
}

DWORD WINAPI ReadThread(LPVOID)
{

    COMSTAT comstat;

    DWORD btr, temp, mask, signal;
    overlapped.hEvent = CreateEvent (NULL, true, true, NULL);

    SetCommMask(hComm, EV_RXCHAR);
    while(1)
    {
        WaitCommEvent(hComm, &mask, &overlapped);

        signal = WaitForSingleObject(overlapped.hEvent, INFINITE);
        if (signal == WAIT_OBJECT_0)
        {
            if (GetOverlappedResult(hCom, &overlapped, &temp, true))
            {
                if((mask & EV_RXCHAR) != 0)
                {
                    ClearCommError(hCommm, &temp, &comstat);
                    btr = comstat.cbInQue;
                    if(btr)
                    {
                        ReadFile(hCom, bufrd, btr, &temp, &overlapped);
                        for (i = 0; i < temp; i++)
                        {
                            printf("%X", bufrd[i]);
                            printf(";");
                        }
                        printf("\n");
                    }
                }
            }
        }
    }
    CloseHandle(overlapped.hEvent);
}

