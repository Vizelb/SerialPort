#ifndef PORT_MENU_H
#define PORT_MENU_H

#include <string.h>
#include <inttypes.h>

#include <windows.h>

#define READ_TIME_COMMAND   ((int) 100)     // const int
#define READ_TIME_DATA      ((int) 500)

#define SERIAL_PORT         "COM4"
#define BOAD_RATE           CBR_19200
//#define BOAD_RATE           ((int) 19200)
//#define SERIAL_PORT         ((LPCTSTR) COM4)

#define PLIS1               ((uint32_t) 0x60)
#define PLIS2               ((uint32_t) 0x70)
#define PLIS_CYCLONE        ((uint32_t) 0x80)
#define PLIS3               ((uint32_t) 0x90)
#define PLIS4               ((uint32_t) 0xA0)

#define ALL_SET1            ((uint32_t) 0xB0)
#define ALL_SET2            ((uint32_t) 0xC0)

// Methods
void InitPortMenu();

// 1 variant
void OpenSerialPort();
void ConfigurateSerialPort();



// 2 variant
BOOL WorkCycle();
BOOL init_com_port();
BOOL config_com_port();
BOOL send_command(uint8_t *command);
BOOL send_data(uint8_t *dataArray, uint16_t arraySize);
//BOOL read_com_port();                 // old
void close_com_port();


BOOL read_data_array_com_port(uint8_t *answerMk);
BOOL read_command_com_port(uint8_t *data);
void CancelFunctiontIoEx();



#endif // PORT_MENU_H
