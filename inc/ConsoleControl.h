#ifndef CONSOLE_CONTROL_H
#define CONSOLE_CONTROL_H

#include <string.h>
#include <inttypes.h>

#include <windows.h>

void InitConsoleControl();

int DuConsoleCommand(void);
void CommandPoPlisDuUpdate();       // 1
int ConsoleCommandDkDriverUpdate(void);    // 2

int JustInputConsoleCommand(void);


#endif
