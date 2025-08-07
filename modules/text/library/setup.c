// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      setup.c
//      Purpose :   Initialisation code
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "text_module.h"
#include "text_module_local.h"

TXTWINDOW defText;

/**
 * @brief      Initialise the text window module and set the default to uninitialised.
 */
void TXTInitialise(void) {
    GFXInitialise();
    defText.isInitialised = false;
}

/**
 * @brief      Write character to default window
 *
 * @param[in]  ch    Character to write.
 */
void TXTWrite(uint32_t ch) {
    TXTWriteWindow(&defText,ch);
}

/**
 * @brief      Get default window structure
 *
 * @return     text window structure
 */
TXTWINDOW *TXTDefault(void) {
    TXTCheckStatus(&defText);
    return &defText;
}

/**
 * @brief      Initialise window to default if not specified.
 *
 * @param      txt   text window structure.
 */
void TXTCheckStatus(TXTWINDOW *txt) {
    if (!txt->isInitialised || DVIGetModeInformation()->mode != txt->currentMode) {
        txt->isInitialised = true;
        TXTSetDefault(txt);
    }
}

/**
 * @brief      Set a window to the default state.
 *
 * @param      txt   Window structure.
 */
void TXTSetDefault(TXTWINDOW *txt) {
    DVIMODEINFO *m = DVIGetModeInformation();
    txt->currentMode = m->mode;
    txt->xSize = txt->ySize = 8;
    txt->xPos = txt->yPos = 0;
    txt->width = m->width / txt->xSize;txt->height = m->height / txt->ySize;
    txt->xCursor = txt->yCursor = 0;
    txt->isCursorDrawn = false;
    txt->textColour = COL_GREEN;txt->backColour = COL_BLACK;
    txt->stateManager = NULL;    
}