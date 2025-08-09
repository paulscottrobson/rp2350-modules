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

#include "screeneditor_module.h"
#include "screeneditor_module_local.h"

SEDSTATUS sedInfo;

/**
 * @brief      Initialise screen editor
 */
void SEDInitialise(void) {
    DVIInitialise();
    INPInitialise();
    MEMInitialise();
    uint8_t *mem = MEMAlloc(SED_MAXWIDTH*SED_MAXHEIGHT+SED_MAXHEIGHT,MEM_ANY);
    LOG("%p",mem);
    if (mem == NULL) ERROR("Couldn't allocate screen memory");
    sedInfo.charMem = mem;
    sedInfo.extendLine = mem + SED_MAXHEIGHT*SED_MAXWIDTH;
    SEDReset();
}

/**
 * @brief      Reset screen editor to use current mode, and clear display.
 */
void SEDReset(void) {
    DVIMODEINFO *m = DVIGetModeInformation();
    sedInfo.x = sedInfo.y = 0;
    sedInfo.width = m->width/8;sedInfo.height = m->height/8;
    sedInfo.xCursor = sedInfo.yCursor = 0;
    sedInfo.colour = (m->bitPlaneDepth == 1) ? 3 : 0x0B;
}
