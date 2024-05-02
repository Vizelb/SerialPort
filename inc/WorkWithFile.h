#ifndef WORK_WITH_FILE_H
#define WORK_WITH_FILE_H

#include <inttypes.h>
#include <string.h>
#include <windows.h>





//  NewMain
BOOL WorkWithFileDuPoUpdate(uint16_t dataSize);                  // �������� �� ����
BOOL InitWorkWithFile(uint32_t currentPlis);    // ��

BOOL ReadFromFile(uint16_t dataSize);
BOOL TransmitPartOfProshivka(uint8_t *dataArray, uint16_t arraySize, uint8_t *answerMk);
uint32_t GetFileSizeMy();

BOOL OpenFilePLISDuPoUpdate();                  // ��������� ���� ��� �������� �� ���� �� ���������
BOOL OpenFileForPort(uint32_t currentPlis);     // ��������� ���� ��� �������� �� ���� �� ��





#endif
