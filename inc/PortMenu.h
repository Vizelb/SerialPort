#ifndef PORT_MENU_H
#define PORT_MENU_H

#include <string.h>
#include <inttypes.h>

#include <windows.h>

#define SEIAL_PORT      "COM12"


// Methods
void InitPortMenu();

// 1 variant
void OpenSerialPort();
void ConfigurateSerialPort();



// 2 variant
void TransmissionCycle();
BOOL init_com_port();
BOOL config_com_port();
BOOL send_char(uint8_t c);
BOOL send_array(uint8_t *dataArray, uint16_t arraySize);
BOOL read_com_port();
void close_com_port();

#endif // PORT_MENU_H
