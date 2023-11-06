#ifndef PORT_MENU_H
#define PORT_MENU_H

#include <string.h>
#include <inttypes.h>

#include <windows.h>

#define SERIAL_PORT         "COM4"
#define BOAD_RATE           CBR_19200
//#define BOAD_RATE           ((int) 19200)
//#define SERIAL_PORT         ((LPCTSTR) COM4)

// Methods
void InitPortMenu();

// 1 variant
void OpenSerialPort();
void ConfigurateSerialPort();



// 2 variant
void WorkCycle();
BOOL init_com_port();
BOOL config_com_port();
BOOL send_char(uint8_t c);
BOOL send_array(uint8_t *dataArray, uint16_t arraySize);
BOOL read_com_port();
void close_com_port();

void repeatWork();

#endif // PORT_MENU_H
