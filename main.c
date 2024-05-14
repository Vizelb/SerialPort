#include <stdio.h>
#include <stdlib.h>

#include "inc/LoaderControl.h"
#include "inc/PortMenu.h"
#include "inc/ConsoleControl.h"
#include "inc/WorkWithFile.h"
#include "inc/CreateFile.h"
#include "inc/Bootloader.h"

int main()
{
    int i = 1;

    //mainBootloader();
    //ReadShum();   // Func for read buff for check BA285
    newMainFunc();    // MAIN NEW WORK FUNCTION v2.0
    //InitCreateFile();



    return 0;
}
