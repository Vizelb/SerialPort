#ifndef CONSOLE_CONTROL_H
#define CONSOLE_CONTROL_H

#include <string.h>
#include <inttypes.h>

#include <windows.h>

void InitConsoleControl();

int DuConsoleCommand(void);
void FormCommandPoPlisDuUpdate();                       // 1
void CommandPoPlisDuProtocol();                         // 2
int ConsoleCommandDkDriverUpdate(void);                 // 3
void FormCommandPoPlisDuUpdate_advancedMode(void);      // 4

int JustInputConsoleCommand(void);


#endif
