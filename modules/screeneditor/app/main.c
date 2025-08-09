#include "screeneditor_module.h"

/**
 * @brief      Main program
 *
 * @return     0 if no error.
 */

int MAINPROGRAM() {
    char buffer[128];
    SEDInitialise();                                                                // Initialise Screen Editor
    DVISetMode(MODE_320_240_64);
    SEDReset();
    sedInfo.x = 1;sedInfo.y = 1;
    sedInfo.width -=2 ;sedInfo.height -= 2;
    SEDClear();
    while (COMAppRunning()) {                                                                     
        while (SEDProcess(buffer,sizeof(buffer),INPGetKey())) COMUpdate();                         
        LOG("Entered '%s'",buffer);
    }
    return 0;
}

