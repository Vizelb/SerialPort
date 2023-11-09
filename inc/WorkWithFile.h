#ifndef WORK_WITH_FILE_H
#define WORK_WITH_FILE_H

#include <inttypes.h>
#include <string.h>
#include <windows.h>


BOOL InitWorkWithFile();
BOOL OpenFileForPort();
BOOL ReadFromFile();

void GetFileSizeMy();

int hex_to_int(char c);
int hex_to_ascii(char c, char d);

int hexChrBin(const char hex, char *out);
void to_hex_16(char *output, unsigned n);


#endif
