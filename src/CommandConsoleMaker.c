#include "../inc/CommandConsoleMaker.h"
#include "../inc/LoaderControl.h"

extern uint8_t rpzuNumber;
extern uint8_t plisNum;
extern uint8_t plisType;
extern uint8_t rpzuFileNumber;
extern uint8_t currentBlock;

extern uint32_t sizeFilePlis;
extern uint32_t addrFilePlisInDk;

struct CommandDu commandDu;

void CommandConsoleMaker(int consoleCommand, uint8_t *data)
{
    if (consoleCommand == 1)
        FormCommandLoadDuPoPlis(data);
    else return;

}

void FormCommandLoadDuPoPlis(uint8_t *data)
{
    int i;
    uint32_t crc32;

    commandDu.command.bytes.code = 0xB1;
    commandDu.command.bytes.second_byte.bits.AMP = currentBlock;
    commandDu.command.bytes.second_byte.bits.EMPTY = 0;
    commandDu.command.bytes.second_byte.bits.SBP = 0;
    commandDu.command.bytes.second_byte.bits.EMPTY2 = 0;

    commandDu.command.bytes.third_byte.bits.NumRPZU = rpzuNumber - 1;
    commandDu.command.bytes.third_byte.bits.EMPTY = 0;

    commandDu.command.bytes.four_byte.bits.NumPLIS = plisNum - 1;
    commandDu.command.bytes.four_byte.bits.TypePLIS = plisType - 1;
    commandDu.command.bytes.four_byte.bits.NumFileRPZU = rpzuFileNumber - 1;

    for (i = 0; i < 6; i++)
        commandDu.command.bytes.emptyBytes[i] = 0x00;

    crc32 = CRC32(commandDu, 10);
    for (i = 0; i < 4; i++)
        commandDu.command.bytes.Crc32[i] = crc32 >> (i*8);

    for (i = 0; i < 14; i++)
        data[i] = commandDu.command.value[i];
}


void FormCommandDkDriverUpdate(uint8_t *command, uint32_t currentPlis)
{
    int i;
    uint32_t crc32;
    struct CommandToMkDkUpdate headCommand;

    headCommand.command.bytes.code = 0x51;
    headCommand.command.bytes.plisNumber = currentPlis;

    for (i = 0; i < 4; i++)
        headCommand.command.bytes.sizeFile[i] = sizeFilePlis >> (i*8);

    for (i = 0; i < 4; i++)
        headCommand.command.bytes.addrDk[i] = addrFilePlisInDk >> (i*8);

    crc32 = CRC32(headCommand, 10);
    for (i = 0; i < 4; i++)
        headCommand.command.bytes.Crc32[i] = crc32 >> (i*8);

    for (i = 0; i < 14; i++)
        command[i] = headCommand.command.value[i];
}




















