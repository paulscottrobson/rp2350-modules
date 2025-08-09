#include "screeneditor_module.h"

/**
 * @brief      Main program
 *
 * @return     0 if no error.
 */
int MAINPROGRAM() {
    SEDInitialise();                                                                // Initialise Screen Editor
    DVISetMode(MODE_640_480_8);
    SEDReset();
    sedInfo.x = 1;sedInfo.y = 1;
    sedInfo.width -=2 ;sedInfo.height -= 2;
    
    while (COMAppRunning()) {                                                                     
        COMUpdate();                         
    }
    return 0;
}

