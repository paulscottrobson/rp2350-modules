// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      textio.c
//      Purpose :   Text I/O functions.
//      Date :      13th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

/**
 * @brief      Set the text foreground/background colour
 *
 * @param[in]  colour  Colour in bits 0..6, foreground bit 7 = 0, background bit 7 = 1
 */
void VDUSetTextColour(int colour) {
    if (colour & 0x80) {
        vc.bgCol = colour & 0x7F;
    } else {
        vc.fgCol = colour & 0x7F;
    }
}

/**
 * @brief      Set the text size. Currently only supports double height.
 *
 * @param[in]  xSize  x Scale
 * @param[in]  ySize  y Scale.
 */
void VDUSetTextSize(uint8_t xSize,uint8_t ySize) {
    vc.textWidth = 8;
    vc.textHeight = (ySize == 2) ? 16:8;
}

/**
 * @brief      VDU Cursor command
 *
 * @param[in]  c     Command (8,9,10,11 or 13)
 */
void VDUCursor(int c) {
    switch(c) {
        case 8:                                                                     // VDU 8 left
            vc.xCursor--;
            if (vc.xCursor < 0) {                                                   // Off the left
                vc.xCursor = vc.tw.xRight-vc.tw.xLeft;                              // Go to EOL 
                VDUCursor(11);                                                      // And up
            }
            break;
        case 9:                                                                     // VDU 9 right
            vc.xCursor++; 
            if (vc.xCursor > vc.tw.xRight-vc.tw.xLeft) {                            // Off the write
                vc.xCursor = 0;                                                     // Go to SOL
                VDUCursor(10);                                                      // And down.
            }
            break;
        case 10:                                                                    // VDU 10 down
            vc.yCursor++;                                                              
            if (vc.yCursor > vc.tw.yBottom-vc.tw.yTop) {                            // Vertical scroll up
   	            VDUScrollV(vc.tw.yTop+1,vc.tw.yTop,vc.tw.yBottom+1,vc.tw.yBottom,vc.tw.xLeft,vc.tw.xRight); 
                vc.yCursor--;
                VDUScrollTextEndMarkers(-1);                                        // Scroll text end markers as well.
            }
            break;
        case 11:                                                                    // VDU 11 up.
            vc.yCursor--;                                                              
            if (vc.yCursor < 0) {
      	        VDUScrollV(vc.tw.yBottom,vc.tw.yBottom+1,vc.tw.yTop,                // Vertical scroll down.
                                    vc.tw.yTop,vc.tw.xLeft,vc.tw.xRight);                   
                vc.yCursor = 0;
                VDUScrollTextEndMarkers(1);                                         // Scroll text end markers as well.
            }
            break;
        case 13:                                                                    // VDU 13 does not go down a line.
            vc.xCursor = 0; 
            break;
    }
}


/**
 * @brief      Character output to text display
 *
 * @param[in]  c     Character to output (non control)
 */
void VDUWriteText(uint8_t c) {
    VDURenderCharacter(vc.xCursor+vc.tw.xLeft,vc.yCursor+vc.tw.yTop,c);            // Write character
    if (vc.xCursor == vc.tw.xRight-vc.tw.xLeft) {                                  // Written to the right hand column, mark extended.
        VDUSetTextEndMarker(vc.yCursor+vc.tw.yTop);                     
    }
    VDUWrite(9);                                                                   // Move forward.
}

/**
 * @brief      Reset the default text colours.
 */
void VDUSetDefaultTextColour(void) {
    VDUSetTextColour(0x07);
    VDUSetTextColour(0x80);
}

