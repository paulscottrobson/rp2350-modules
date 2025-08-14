// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      scaling.c
//      Purpose :   Scaling and cursor and window functions.
//      Date :      13th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************

#include "graphics_module.h"
#include "graphics_module_local.h"

/**
 * @brief      Set the default graphics colours.
 */
void VDUSetDefaultGraphicColour(void) {
    VDUSetGraphicsColour(0,7);                                                      // GCOL 0,7
    VDUSetGraphicsColour(0,0x80);                                                   // GCOL 0,128
}

/**
 * @brief      Reset the graphics cursor information
 */
void VDUResetGraphicsCursor(void) {
    vc.xLastLogical = vc.yLastLogical = 0;
    for (int i = 0;i < SAVED_COORDS;i++) {
        vc.xCoord[i] = vc.yCoord[i] = 0;
    }
}

/**
 * @brief      Get the background colour
 *
 * @return     Background colour.
 */
int VDUGetBackgroundColour(void) {
    return vc.bgrGraphic;
}

/**
 * @brief      Set the graphic mode and colour (this is GCOL)
 *
 * @param[in]  mode    The mode
 * @param[in]  colour  The colour
 */

void VDUSetGraphicsColour(uint8_t mode,uint8_t colour) {
    vc.gColMode = mode;                                                             // Save mode. According to MOS1.2 this is the same mode for both
    if (colour & 0x80) {                                                            // If bit 7 set, background
        vc.bgrGraphic = colour & 0x7F;
    } else {                                                                        // If bit 7 clear, foreground
        vc.fgrGraphic = colour & 0x7F;
    }
}


/**
 * @brief      Set the graphics origin
 *
 * @param[in]  x     Logical x position of origin
 * @param[in]  y     Logical y position of origin
 */
void VDUSetGraphicsOrigin(int x,int y) {
    vc.xOrigin = x;vc.yOrigin = y;
}


/**
 * @brief      Return pixel at logical coordinates x,y
 *
 * @param[in]  x     x logical coordinate
 * @param[in]  y     y logical coordinate
 *
 * @return     colour if +ve, -1 if invalid (out of screen/window)
 */
int  VDUReadPixel(int32_t x,int32_t y) {
    x = x >> vc.xScale;y = y >> vc.yScale;                                          // Scale to physical coordinates
    if (x < vc.gw.xLeft || y < vc.gw.yBottom ||                                     // Check out of window area.
                x > vc.gw.xRight || y > vc.gw.yTop) return -1;              
    return VDUAReadPixel(x,y,false);                                                // Call the atomic function
}

/**
 * @brief      Do a Plot command
 *
 * @param[in]  cmd   Command ID
 * @param[in]  x     Logical X coordinate
 * @param[in]  y     Logical Y coordinate
 */
void VDUPlot(uint8_t cmd,int32_t x,int32_t y) {

    VDUHideCursor();

    //
    //      Handle offset (e.g. command bit 2 is zero)
    //
    if ((cmd & 4) == 0) {                                                           // Is it a relative movement.
        x += vc.xLastLogical;y += vc.yLastLogical;                                  // If so, offset from the last logical position
    }
    vc.xLastLogical = x;vc.yLastLogical = y;                                        // Update the last logical position.
    //
    //      Convert to physical, first offset by the origin
    //
    x += vc.xOrigin; y += vc.yOrigin;                                               // Adjust for origin.
    //
    //      Push the old coordinates on the stack for things like triangles
    //
    vc.xCoord[2] = vc.xCoord[1];vc.yCoord[2] = vc.yCoord[1];                        // Push x and y cordinates down the 3 level store
    vc.xCoord[1] = vc.xCoord[0];vc.yCoord[1] = vc.yCoord[0];
    //
    //      And add the coordinate in physical pixels from (0,0) at bottom left of screen.
    //
    vc.xCoord[0] = x >> vc.xScale;vc.yCoord[0] = y >> vc.yScale;                    // Add the latest coordinate.
    //printf("Plot: %d %d,%d\n",cmd,vc.xCoord[0],vc.yCoord[0]);

    //
    //      Now workout the GCOL data, both the action and colour (the two GCOL parameters)
    //
    int drawMode = cmd & 3;                                                         // 0 Move only, 1 Background, 2 Inverse, 3 Foreground
    int command = cmd & 0xF8;                                                       // Command byte, ignores lower 3 bits.

    if (drawMode == 0) return;                                                      // Move only, exit.
    VDUSetDrawingData(drawMode);                                                    // Set the colour drawing.
    //
    //      Now work out special draws, dotted and missing end point on inversion draws only.
    //
    int controlBits = 0;
    if (command == 16 || command == 24) controlBits |= GFXC_DOTTED;                 // $10 $18 are the same as $00 and $08
    if (command == 8 || command == 24) {                                            // Possible inverting ?
        if (drawMode == 2 || vc.gColMode == 4) controlBits |= GFXC_NOENDPOINT;      // If inverting, do not draw end point.        
    }
    VDUASetControlBits(controlBits);                                                // Set the various control bits.
    VDUPlotDispatch(command,vc.xCoord,vc.yCoord);                                   // Go do it
}

/**
 * @brief      Set up the graphic drawing bytes
 *
 * @param[in]  drawMode  Draw mode (1,2,3 as in PLOT)
 */
void VDUSetDrawingData(int drawMode) {
   VDUASetActionColour(vc.gColMode,                                                 // Tell it the action and the colour
                        (drawMode == 1) ? vc.fgrGraphic:vc.bgrGraphic);
    if (drawMode == 2) {                                                            // Invert
        VDUASetActionColour(3,0x7F);                                                // Set EOR $7F on colour.
    }
}

/**
 * @brief      Handle cursor movements in VDU 5 mode
 *
 *             Wraps around in the Window, 
 *
 * @param[in]  c     Cursor movements (8,9,10,11,13)
 */
void VDUGCursor(int c) {
    switch(c) {
        case 8:                                                                     // Left
            vc.xCoord[0] -= 8;
            if (vc.xCoord[0] < vc.gw.xLeft) {
                VDUGCursor(11);vc.xCoord[0] = vc.gw.xRight - 8;
            }
            break;
        case 9:                                                                     // Right
            vc.xCoord[0] += 8;
            if (vc.xCoord[0] > vc.gw.xRight) {
                VDUGCursor(10);VDUGCursor(13);
            }
            break;
        case 10:                                                                    // Down
            vc.yCoord[0] -= 8;
            if (vc.yCoord[0] < vc.gw.yBottom) vc.yCoord[0] = vc.gw.yTop;
            break;
        case 11:                                                                    // Up
            vc.yCoord[0] += 8;
            if (vc.yCoord[0] > vc.gw.yTop) vc.yCoord[0] = vc.gw.yBottom + 8;
            break;
        case 13:                                                                    // Carriage Return
            vc.xCoord[0] = vc.gw.xLeft;
            break;
    }
}

/**
 * @brief      Output character in VDU 5 mode
 *
 * @param[in]  c     Character code to output.
 */
void VDUGWriteText(int c) {
    VDUSetDrawingData(1);                                                           // Use foreground mode
    for (int y = 0;y < 8;y++) {
        VDUAOutputByte(vc.xCoord[0],vc.yCoord[0]-y,VDUGetCharacterLineData(c,y,false));
    }
    VDUGCursor(9);                                                                  // Forward one.
}