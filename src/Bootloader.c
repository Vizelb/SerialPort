#include "../inc/Bootloader.h"
#include "../inc/PortMenu.h"





void mainBootloader()
{
    if (!Synchronization())
        return;
    if (!SetSpeed(115200))
        return;

}






BOOL Synchronization()
{
    int i;
    uint8_t tx[1];
    uint8_t rx[3];

    if (!SetSettingsComPort(COM_PORT_9, CBR_9600))
        return FALSE;

    tx[0] = 0x00;
    SendData(tx, 1);
    ReadData(rx, 3, READ_TIME_COMMAND);
    if (rx[0] != (char)0x0d || rx[1] != (char)0x0a || rx[2] != (char)0x3e)
        return FALSE;
    return TRUE;
}

BOOL SetSpeed(uint32_t baudRate)
{
    uint8_t tx[14];
    uint8_t rx[14];

    tx[0] = 'B';
    tx[1] = (uint8_t*) (&baudRate)[0];     // 0x0;
    tx[2] = (uint8_t*) (&baudRate)[1];     // (char)0xc2;
    tx[4] = (uint8_t*) (&baudRate)[3];     // 0x00;
    tx[3] = (uint8_t*) (&baudRate)[2];     // 0x01;

    SendData(tx, 5);
    ReadData(rx, 1, READ_TIME_COMMAND);
    sleep(100);
    close_com_port;
    if (!SetSettingsComPort(COM_PORT_9, CBR_115200))
        return FALSE;

    tx[0] = (char)0x0D;
    SendData(tx, 1);
    ReadData(rx, 3, READ_TIME_COMMAND);
    if (rx[0] != (char)0x0d || rx[1] != (char)0x0a || rx[2] != (char)0x3e)
        return FALSE;
    return TRUE;
}
