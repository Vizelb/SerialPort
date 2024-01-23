#ifndef __CRC32_
#define __CRC32_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <windows.h>



uint32_t CRC32(uint8_t *buf, uint32_t length);
BOOL CheckCRC32(uint32_t expected, uint8_t *actual);

#endif  // __CRC32_
