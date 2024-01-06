#include "../inc/CommandConsoleMaker.h"
#include "../inc/LoaderControl.h"

extern uint8_t rpzuNumber;
extern uint8_t plisNum;
extern uint8_t plisType;
extern uint8_t rpzuFileNumber;
extern uint8_t currentBlock;

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



