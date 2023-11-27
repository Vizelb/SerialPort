#include "../inc/CRC32.h"

#include <stdio.h>
#include <stdlib.h>

/* Rasschet CRC32
buf - ��������� �� ������
length - �����
*/
uint32_t CRC32(uint8_t *buf, uint32_t length)
{
    uint32_t crc = 0xFFFFFFFF;
    while (length--)
        crc = (crc >> 8) ^ crc32Table[(crc ^ *buf++) & 0xFF];
    return crc ^ 0xFFFFFFFF;
}

/*��������� ������������ CRC32(expected) � ����������(actual)
���������� TRUE, ���� ���������, ����� FALSE*/
BOOL CheckCRC32(uint32_t expected, uint8_t *actual)
{
    int j, i ;
    uint8_t temp;
    for (i = 0, j = 0; j < 4; i++, j++)
    {
        temp = (expected >> (8*i));
        if(actual[j] != temp)
            return FALSE;
    }
    return TRUE;
}


