#ifndef CONSOLE_CONTROL_H
#define CONSOLE_CONTROL_H

#include <string.h>
#include <inttypes.h>

#include <windows.h>

void InitConsoleControl();
uint8_t InputCommand();

BOOL CheckCurrentPlis(uint32_t command, uint32_t *currentPlis);


#endif
