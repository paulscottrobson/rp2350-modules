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
 
#include "alt_graphics_module.h"
#include "alt_graphics_module_local.h"

static void *pointerData = NULL;                                                    // Used as temp for GFXDrawP()
static void GFXClear(void);
static uint32_t GFXMap12BitColour(uint32_t colour);

/**
 * @brief      Execute a graphics command. If different signature/same
 *             identifier works this may be just GFXDraw(*,#), but for now this
 *             always works.
 *
 * @param[in]  cmd   command ID
 * @param      p     Pointer being passed in.
 * @param[in]  y     y Coordinate
 *
 * @return     Return value depends.
 */
uint32_t GFXDrawP(enum GFXCommand cmd,void *p,uint32_t y) {    
    pointerData = p;                                                                // Save the pointer
    return GFXDraw(cmd,0,y);                                                        // And call the main drawer
}

/**
 * @brief      Execute a graphics command
 *
 * @param[in]  cmd   command ID
 * @param[in]  x     x Coordinate
 * @param[in]  y     y Coordinate
 * 
 * @return     Return value depends.
 */
uint32_t GFXDraw(enum GFXCommand cmd,uint32_t x,uint32_t y) {    

    uint32_t oldFgr,retVal = 0;
    switch(cmd) {
        //
        //      Control functions
        //
        case Mode:                                                                  // Set screen mode
            DVISetMode(x);
            GFXCheckModeChange();
            GFXClear();
            break;

        case RawColour:                                                             // Set Colour (raw)
        case Colour:                                                                // Set Colour (12-bit)
            if (cmd == Colour) {                                                    // 12 bit colour, map to nearest.
                x = GFXMap12BitColour(x);
                y = GFXMap12BitColour(x);
            }
            draw->foreground = x;
            draw->background = y;
            draw->isTransparent = false;  
            if ((y & 0xFFFF) == 0xFFFF) {                                           // Background transparent.
                draw->background = 0;draw->isTransparent = true; 
            }
            break;

        case DrawMode:                                                              // Set the drawing mode.
            draw->drawMode = x;break;
            
        case Scaling:                                                               // Set font scaling.
            draw->xFontScale = x;draw->yFontScale = y;
            break;

        case SetClip:                                                               // Set current clip
            GFXPreProcess(&x,&y);
            draw->clip.xLeft = x;draw->clip.yTop = y;                               // Set the clipping rect like a rectangle draw
            draw->clip.xRight = draw->xPrev[0];draw->clip.yBottom = draw->yPrev[0];
            SORT_PAIR(draw->clip.xLeft,draw->clip.xRight);                          // Sort clip rect, as we might have reversed x/y axis.
            SORT_PAIR(draw->clip.yTop,draw->clip.yBottom);
            break;

        case NoClip:                                                                // Remove clipping
            GFXResetClipping();
            break;

        case SetMapper:                                                             // Set the mapping from logical to physical
            draw->mapper = (GFXMAPPER)pointerData;                                  // (by default logical == physical)
            break;
        //
        //      Drawing functions
        //
        case Move:                                                                  // Move to location
            GFXPreProcess(&x,&y);
            GFXRawMove(x,y);
            break;

        case Plot:                                                                  // Move to location and plot pixel.
            GFXPreProcess(&x,&y);
            GFXRawMove(x,y);
            GFXRawPlot(true);
            break;

        case Line:                                                                  // Draw a solid line.
            GFXPreProcess(&x,&y);
            GFXDrawLine(draw->xPrev[0],draw->yPrev[0],x,y,true);
            break;

        case Rect:                                                                  // Rectangles
        case FillRect:
            GFXPreProcess(&x,&y);
            GFXDrawRectangle(draw->xPrev[0],draw->yPrev[0],x,y,cmd == FillRect);
            GFXRawMove(x,y);
            break;

        case Ellipse:                                                               // Ellipses
        case FillEllipse:
            GFXPreProcess(&x,&y);
            GFXDrawEllipse(draw->xPrev[0],draw->yPrev[0],x,y,cmd == FillEllipse);
            GFXRawMove(x,y);
            break;

       case Triangle:                                                               // Outline triangle
            GFXPreProcess(&x,&y);
            GFXDrawOutlineTriangle(draw->xPrev[1],draw->yPrev[1],draw->xPrev[0],draw->yPrev[0],x,y);
            GFXRawMove(x,y);
            break;

        case FillTriangle:                                                          // Filled triangle
            GFXPreProcess(&x,&y);
            GFXDrawFilledTriangle(draw->xPrev[1],draw->yPrev[1],draw->xPrev[0],draw->yPrev[0],x,y);
            GFXRawMove(x,y);
            break;

        case Character:                                                             // Draw a character
            uint32_t xOrg = draw->x,yOrg = draw->y;
            x = GFXDrawCharacter(draw->x,draw->y,x);
            GFXRawMove(xOrg+(x & 0xFF),yOrg);            
            break;

        case CharExtent:                                                            // Get character extent.
            retVal = GFXGetCharacterExtent(x);                                      // Height in upper 16 bits, Width in lower1 16 bits
            break;

        case Clear:                                                                 // Clear whole screen to background
            GFXClear();
            break;

        case ClearWindow:                                                           // Clear the window to background
            oldFgr = draw->foreground;draw->foreground = draw->background;
            for (int y = draw->clip.yTop;y <= draw->clip.yBottom;y++) {
                GFXDraw(Move,draw->clip.xLeft,y);
                GFXDraw(Line,draw->clip.xRight,y);
            }
            draw->foreground = oldFgr;
            break;

        case Desktop:                                                               // Clear whole screen to desktop
            GFXDrawDesktop();
            break;          
    }
    return retVal;
}

/**
 * @brief      Preprocess coordinates. Does logical->physical conversion and
 *             tracks previous coordinates.
 *
 * @param      x     address of x
 * @param      y     address of y
 */
void GFXPreProcess(int32_t *x,int32_t *y) {
    // Logical to Physical mapping.
    if (draw->mapper != NULL) {
        (*draw->mapper)(x,y);
    }
    // Push coordinates onto previous lists.
    draw->xPrev[2] = draw->xPrev[1];draw->yPrev[2] = draw->yPrev[1];                    
    draw->xPrev[1] = draw->xPrev[0];draw->yPrev[1] = draw->yPrev[0];
    draw->xPrev[0] = draw->x;       draw->yPrev[0] = draw->y;
}


/**
 * @brief      Screen clear
 */
static void GFXClear(void) {
    uint32_t oldFgr = draw->foreground;draw->foreground = draw->background;
    for (int y = 0;y < modeInfo.height;y++) {
        GFXDraw(Move,0,y);
        GFXDraw(Line,modeInfo.width-1,y);
    }
    draw->foreground = oldFgr;
}

/**
 * @brief      Map 12 bit colour (BBBBGGGGRRRR) to current mode
 *
 * @param[in]  colour  Colour in 12 bit format
 *
 * @return     Colour in raw format.
 */
static uint32_t GFXMap12BitColour(uint32_t colour) {
    uint8_t r = (colour == 0) ? 0 : 0xFF;
    switch(DVIGetModeInformation()->bitPlaneDepth) {
        case 1:
            r = ((colour & 0x800) >> 9) + ((colour && 0x080) >> 6) + ((colour & 0x008) >> 3);
            break;
        case 2:
            r = ((colour & 0xC00) >> 6) + ((colour & 0x0C0) >> 4) + ((colour & 0x00C) >> 2);
            break;

    }
    return r;
}
