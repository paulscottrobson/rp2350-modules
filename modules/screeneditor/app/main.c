#include "screeneditor_module.h"

/**
 * @brief      Main program
 *
 * @return     0 if no error.
 */
int MAINPROGRAM() {
    DVIInitialise();                                                                // Initialise Graphics    
    DVISetMode(MODE_640_480_8);
    INPInitialise();
    MEMInitialise();

    while (COMAppRunning()) {                                                                     
        COMUpdate();                         
    }
    return 0;
}

