#include "screen_module.h"

/**
 * @brief      Main program
 *
 * @return     0 if no error.
 */

int MAINPROGRAM() {
    char buffer[256];
    SCRInitialise();                                                                // Initialise Screen Editor
    DVISetMode(MODE_640_480_8);
    SCRReset();
    scrInfo.x = 1;scrInfo.y = 1;
    scrInfo.width -= 2;
    scrInfo.height -= 2;
    SCRWriteChar(CTL_CLEAR);
    for (int i = 32;i < 256;i++) SCRWriteChar(i);
    while (COMAppRunning()) {                                                                     
        while (SCRProcess(buffer,sizeof(buffer),INPGetKey())) COMUpdate();                         
        LOG("Entered '%s'",buffer);
    }
    return 0;
}

