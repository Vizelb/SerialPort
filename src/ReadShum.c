#include "../inc/ReadShum.h"
#include "../inc/PortMenu.h"
#include "../inc/CRC32.h"


uint8_t dataBuff[SIZE_DATA_TO_PC + 4];
extern const uint32_t crc32Table[256];



void ReadShum()
{
    int i, j;
    int counterPackage = 0;
    uint32_t dataWrote;
    uint32_t crcINT2 = 0xFFFFFFFF;

    FILE *file;

    file = fopen("D:/Danya/Libary/C/newFileShum.bin", "wb");

    printf("Start\n");
    if (!SetSettingsComPort(COM_PORT_9, CBR_9600))
        return;

    printf("Start Work\n");

    while (1)
    {
        if(!read_data_array_com_port_Shum(dataBuff, SIZE_DATA_TO_PC))
            break;

        /*crc32 = CRC32(dataBuff, SIZE_DATA_TO_PC);
        if(!CheckCRC32(crc32, &dataBuff[SIZE_DATA_TO_PC]))
        {
            printf("\n\n ERROR KS");
            //while(1){};
        }*/


        printf(" - %d\n", dataBuff[0]);
        dataWrote = fwrite(dataBuff, sizeof(uint8_t), SIZE_DATA_TO_PC, file);
        counterPackage++;
        printf("\nTotal Bytes Receive = %d, = 0x%X ", counterPackage*SIZE_DATA_TO_PC, counterPackage*SIZE_DATA_TO_PC);
        printf(" - %d\n", dataWrote);

        /*if (counterPackage == 81)
        {
            for (i = 0; i < 5; i++)
            {
                dataBuff[0] = 0x00;
                dataWrote = fwrite(dataBuff, sizeof(uint8_t), SIZE_DATA_TO_PC, file);
            }
        }
        if (counterPackage == 158)
        {
            for (i = 0; i < 5; i++)
            {
                dataBuff[0] = 0x00;
                dataWrote = fwrite(dataBuff, sizeof(uint8_t), SIZE_DATA_TO_PC, file);
            }
        }
        if (counterPackage == 222)
        {
            for (i = 0; i < 5; i++)
            {
                dataBuff[0] = 0x00;
                dataWrote = fwrite(dataBuff, sizeof(uint8_t), SIZE_DATA_TO_PC, file);
            }
        }*/
        /*if (counterPackage == 2000)
        {
            for (i = 0; i < 5; i++)
            {
                dataBuff[0] = 0x00;
                dataWrote = fwrite(dataBuff, sizeof(uint8_t), SIZE_DATA_TO_PC, file);
            }
            fclose(file);
            break;
        }*/


        crcINT2 = (crcINT2 >> 8) ^ crc32Table[(crcINT2 ^ dataBuff[0]) & 0xFF];
        printf("crcINT2 = %8.X\n", crcINT2);
        if (counterPackage == 1250000)
        {
            crcINT2 = crcINT2 ^ 0xFFFFFFFF;
            printf("crcINT2 = %8.X\n", crcINT2);
            counterPackage = 0;
            crcINT2 = 0xFFFFFFFF;
            break;
        }
    }

    close_com_port();
    printf("\nEnd\n");

}
