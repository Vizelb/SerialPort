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
typedef struct CommandToMkDkUpdate
{
    union FormCommandToMk command;
};


struct SecondBitsCommandDu{
    uint8_t
        AMP:3,
        EMPTY:1,
        SBP:2,
        EMPTY2:2;
};
union SecondByteCommandDu{
    uint8_t value;
    struct SecondBitsCommandDu bits;
};

struct ThirdBitsCommandDu{
    uint8_t
        NumRPZU:4,
        EMPTY:4;
};
union ThirdByteCommandDu{
    uint8_t value;
    struct ThirdBitsCommandDu bits;
};

struct FourthBitsCommandDu{
    uint8_t
        NumPLIS     :2,
        TypePLIS    :2,
        NumFileRPZU :4;
};
union FourthByteCommandDu{
    uint8_t value;
    struct FourthBitsCommandDu bits;
};

union FormCommandDu
{
    uint8_t value[14];
    struct
    {
        uint8_t code;
        union SecondByteCommandDu second_byte;
        union ThirdByteCommandDu third_byte;
        union FourthByteCommandDu four_byte;
        uint8_t emptyBytes[6];
        uint8_t Crc32[4];
    } bytes;
};
typedef struct CommandDu
{
    union FormCommandDu command;
};




union AnalysisAnswerFromMk
{
    uint8_t value[14];
    struct
    {
        uint8_t code;
        uint8_t plisNumber;     // change to Sec_Byte
        uint8_t kpk;
        uint8_t emptyBytes[7];
        uint8_t Crc32[4];
    } bytes;
};
typedef struct AnswerFromMk
{
    union AnalysisAnswerFromMk answer;
};

#define K_K_UPR                             ((uint8_t) 0x51)
#define K_K_END                             ((uint8_t) 0x5F)
#define K_KVIT                              ((uint8_t) 0x91)
#define K_R_D                               ((uint8_t) 0x9F)

#define SIZE_PLIS_RUSSIAN           ((uint32_t) 0x12F57)
#define SIZE_PLIS_CYCLONE           ((uint32_t) 0x59D8B)

//  main
void InitLoaderControl();
//
BOOL StartLoadingFile(HANDLE hComm, uint32_t currentPlis);
// #1 First function - KKU to init process
BOOL TransmitCommandControl(HANDLE hComm, uint32_t currentPlis);
void FormCommandDu(uint8_t *command, uint32_t currentPlis, uint8_t codeCommand);    // Update
// #2 Second function
BOOL TransmitDataFile(uint32_t currentPlis);
BOOL TransmitPartOfProshivka(uint8_t *dataArray, uint16_t arraySize, uint8_t *answerMk);

BOOL CheckCurrentPlis(uint32_t command, uint32_t *currentPlis);
BOOL CheckAnswerCommand(uint8_t *commandAnswer, uint32_t currentPlis, uint8_t codeCommand);


// New Structura
void newMainFunc(void);
BOOL CommandLoadDuPoPlis(); // Load PO PLIS
void CheckCommandControl(int consoleCommand);

// Testes
void testUartFunc(void);


#endif // LOADER_CONTROL_H
