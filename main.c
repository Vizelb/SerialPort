#include <stdio.h>
#include <stdlib.h>

#include "inc/LoaderControl.h"
#include "inc/PortMenu.h"
#include "inc/ConsoleControl.h"
#include "inc/WorkWithFile.h"
#include "inc/CreateFile.h"

int main()
{
    int i = 0;
/*    uint8_t mass[4] = {0};
    uint32_t test = 0xFFAABBCC;

    *mass = test;
    mass[0] = test;
    mass[1] = test >> 8;
    mass[2] = test >> 16;
    mass[3] = test >> 24;
    printf("\n check = %x\n", mass);
    printf("\n check[0] = %x\n", mass[0]);
    printf("\n check[1] = %x\n", mass[1]);
    printf("\n check[2] = %x\n", mass[2]);
    printf("\n check[3] = %x\n", mass[3]);
*/
    //InitLoaderControl();    // Main Work Function

    InitPortMenu();         // Main Work Function
    //InitLoaderControl();
    //InitConsoleControl();

    //InitWorkWithFile();


    //InitCreateFile();
    /*do
    {
       i += 1;
       printf("I = %d\n", i);
    }   while (i == 10 || i < 10);
    printf("I = %d\n", i);
    */
    return 0;
}
