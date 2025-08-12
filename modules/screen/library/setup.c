// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      setup.c
//      Purpose :   Screen Editor Setup
//      Date :      9th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "screen_module.h"
#include "screen_module_local.h"

SCRSTATUS scrInfo;

/**
 * @brief      Initialise screen editor
 */
void SCRInitialise(void) {
    static bool isInitialised = false;
    if (isInitialised) return;
    isInitialised = true;
    DVIInitialise();
    INPInitialise();
    MEMInitialise();
    uint8_t *mem = MEMAlloc(SCR_MAXWIDTH*SCR_MAXHEIGHT+SCR_MAXHEIGHT,MEM_ANY);
    LOG("%p",mem);
    if (mem == NULL) ERROR("Couldn't allocate screen memory");
    scrInfo.charMem = mem;
    scrInfo.extendLine = mem + SCR_MAXHEIGHT*SCR_MAXWIDTH;
    SCRReset();
}

/**
 * @brief      Reset screen editor to use current mode, and clear display.
 */
void SCRReset(void) {
    DVIMODEINFO *m = DVIGetModeInformation();
    scrInfo.x = scrInfo.y = 0;
    scrInfo.currentMode = m->mode;
    scrInfo.width = m->width/8;scrInfo.height = m->height/8;
    scrInfo.xCursor = scrInfo.yCursor = 0;
    scrInfo.colour = (m->bitPlaneDepth == 1) ? 2 : 0x08;
    scrInfo.cursorColour = (m->bitPlaneDepth == 1) ? 6 : 0x3C;
}
