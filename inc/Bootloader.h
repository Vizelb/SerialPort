#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

#include <string.h>
#include <inttypes.h>
#include <windows.h>

void mainBootloader();
BOOL Synchronization();
BOOL SetSpeed(uint32_t baudRate);

#endif  // __BOOTLOADER_H
