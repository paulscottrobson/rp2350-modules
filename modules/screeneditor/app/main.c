#include "screeneditor_module.h"

/**
 * @brief      Main program
 *
 * @return     0 if no error.
 */
int MAINPROGRAM() {
    SEDInitialise();                                                                // Initialise Screen Editor
    DVISetMode(MODE_320_240_8);
    SEDReset();
    sedInfo.x = 1;sedInfo.y = 1;
    sedInfo.width -=2 ;sedInfo.height -= 2;
    SEDClear();
    while (COMAppRunning()) {                                                                     
        COMUpdate();                         
    }
    return 0;
}

