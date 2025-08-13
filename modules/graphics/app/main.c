#include "graphics_module.h"

static void generalTest(void);

int MAINPROGRAM() {
    DVIInitialise();
    generalTest();
}

/**
 * @brief      General test procedure
 */
static void generalTest(void) {
    VDUWrite(22);
    VDUWrite(MODE_320_240_8);
    VDUPlotCommand(4,0,0);
    VDUPlotCommand(5,640,512);
    for (int i = 32;i < 140;i++) VDUWrite(i);
    while (COMAppRunning()) {                                                                     
        COMUpdate();
    }    
}
