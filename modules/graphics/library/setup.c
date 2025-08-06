// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      setup.c
//      Purpose :   Graphics module setup
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

struct DrawingContext *draw;                                                        // Current drawing state
struct DrawingContext contextStack[MAX_CONTEXTS];
int contextStackPointer;

/**
 * @brief      Initialise the graphics system.
 */
void GFXInitialise(void) {
    static bool isInitialised = false;                                              // Only initialise once.
    if (isInitialised) return;
    isInitialised = true;
    VMDInitialise();                                                                // Initialise Video Modes
    draw = &contextStack[0];                                                        // Set up the default context.
    contextStackPointer = 0;                                                        // Reset the stack of contexts.
    GFXInitialiseDrawStructure();                                                   // Defaults
    draw->currentMode = 0xFFFFFFFF;                                                 // Effectively "no current mode set". Magic constant, so sue me :)
}

/**
 * @brief      Open a new drawing/graphics contexts. Effectively saves the
 *             current drawing state, and opens a new one set at all the
 *             defaults. It works like a stack. 
 *
 * @return     true if ok, false if too many states nested.
 */
bool GFXOpenContext(void) {
    if (contextStackPointer == MAX_CONTEXTS-1) return false;                        // We are on the deepest context
    contextStackPointer++;                                                          // Shift context to next down 
    draw = &contextStack[contextStackPointer];                                      // Make that the active context
    GFXInitialiseDrawStructure();                                                   // Set defaults 
    return true;
}

/**
 * @brief      Close a drawing/graphics contexts.
 *
 * @return     true if ok, false if none to close.
 */
bool GFXCloseContext(void) {
    if (contextStackPointer == 0) return false;                                     // Haven't opened one !
    contextStackPointer--;                                                          // Go to the previous context
    draw = &contextStack[contextStackPointer];  
    return true;
}

/**
 * @brief      Called to check if the mode has changed, it sets up the draw system with the default.
 */
void GFXCheckModeChange(void) {
    if (vi.mode != draw->currentMode) {                                             // Has the mode changed, if so reset graphics for it.        
        draw->currentMode = vi.mode;                                                // Update the current mode.
        GFXInitialiseDrawStructure();
    }
}

/**
 * @brief      Initialise the draw structure
 */
void GFXInitialiseDrawStructure(void) {
    draw->foreground = 0xFF;draw->background = 0;                                   // Default foreground/background colour.
    draw->isTransparent = false;

    draw->xFontScale = draw->yFontScale = 1;                                        // Font scalars
    draw->mapper = NULL;                                                            // No coordinate mapper.
    draw->font = GFXGetSystemCharacter;                                             // Default font.
    draw->drawMode = 0;                                                             // Default draw mode.
    GFXResetClipping();                                                             // No clipping         
    GFXRawMove(0,0);                                                                // Move to the home position.

    for (int i = 0;i < 3;i++) {                                                     // Clear the previous coordinates arrays.
        draw->xPrev[i] = draw->yPrev[i] = 0;
    }
}

/**
 * @brief      Reset all clipping to full screen.
 */
void GFXResetClipping(void) {
    draw->clip.xLeft = 0;draw->clip.xRight = vi.xScreen-1;
    draw->clip.yTop = 0;draw->clip.yBottom = vi.yScreen-1;
}
        
