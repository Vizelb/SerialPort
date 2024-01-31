#ifndef WORK_WITH_FILE_H
#define WORK_WITH_FILE_H

#include <inttypes.h>
#include <string.h>
#include <windows.h>






//  NewMain
BOOL WorkWithFileDuPoUpdate();                  // протокол ДУ ПЛИС
BOOL InitWorkWithFile(uint32_t currentPlis);    // ДК

BOOL ReadFromFile();
BOOL TransmitPartOfProshivka(uint8_t *dataArray, uint16_t arraySize, uint8_t *answerMk);
uint32_t GetFileSizeMy();

BOOL OpenFilePLISDuPoUpdate();                  // Открываем файл для прошивки ПО ПЛИС по протоколу
BOOL OpenFileForPort(uint32_t currentPlis);     // Открываем файл для загрузки ПО ПЛИС на ДК





#endif
