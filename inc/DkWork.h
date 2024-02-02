#ifndef __DK_WORK_H
#define __DK_WORK_H

#include <stdio.h>
#include <stdlib.h>
//#include <stdbool.h>
#include <inttypes.h>
#include <windows.h>



BOOL LoadInDkPlisFirmware();
BOOL CheckCurrentPlis(uint32_t *currentPlis);
BOOL TransmitDataFile(uint32_t currentPlis);



#endif  // __DK_WORK_H
