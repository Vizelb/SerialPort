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

union AnalysisAnswerFromMk
{
    uint8_t value[14];
    struct
    {
        uint8_t code;
        uint8_t plisNumber;
        uint8_t emptyBytes[8];
        uint8_t Crc32[4];
    } bytes;
};
typedef struct AnswerFromMk
{
    union AnalysisAnswerFromMk answer;
};

#define K_K_UPR                         ((uint8_t) 0x51)
#define K_KVIT                          ((uint8_t) 0x91)

#define SIZE_PLIS_RUSSIAN           ((uint32_t) 0x12F57)
#define SIZE_PLIS_CYCLONE           ((uint32_t) 0x59D8B)

// main
BOOL InitLoaderControl(HANDLE hComm, uint32_t currentPlis);
// #1 First function - KKU to init process
BOOL StartConnection(HANDLE hComm, uint32_t currentPlis);
void FormCommand(uint8_t *command, uint32_t currentPlis);

BOOL CheckCurrentPlis(uint32_t command, uint32_t *currentPlis);

BOOL CheckAnswerCommand(uint8_t *commandAnswer, uint32_t currentPlis);

// #2 Second function
BOOL TransmitDataFile(HANDLE hComm);

#endif // LOADER_CONTROL_H
