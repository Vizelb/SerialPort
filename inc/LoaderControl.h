#ifndef LOADER_CONTROL_H
#define LOADER_CONTROL_H

#include <inttypes.h>
#include <windows.h>

union FormCommandToMk
{
    uint8_t value[14];
    struct
    {
        uint8_t code;
        uint8_t plisNumber;
        uint8_t sizeFile[4];
        uint8_t addrDk[4];
        uint8_t Crc32[4];
    } bytes;
    //Command_To_Mk byte;
};
typedef struct CommandToMk
{
    union FormCommandToMk command;
};

#define KKU         ((uint8_t) 0x51)

#define SIZE_PLIS_RUSSIAN           ((uint32_t) 0x12F57)
#define SIZE_PLIS_CYCLONE           ((uint32_t) 0x59D8B)

BOOL InitLoaderControl(HANDLE hComm, uint32_t currentPlis);
void FormCommand(uint8_t *command, uint32_t currentPlis);

BOOL CheckCurrentPlis(uint32_t command, uint32_t *currentPlis);



#endif // LOADER_CONTROL_H
