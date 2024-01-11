#ifndef __COMMAND_CONSOLE_MAKER_
#define __COMMAND_CONSOLE_MAKER_

#include "inttypes.h"

void CommandConsoleMaker(int consoleCommand, uint8_t *data);
void FormCommandLoadDuPoPlis(uint8_t *data);

// Dk driver Update
void FormCommandDkDriverUpdate(uint8_t *command, uint32_t currentPlis);



#endif  // __COMMAND_CONSOLE_MAKER_
