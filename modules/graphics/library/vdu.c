// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      vdu.c
//      Purpose :   Main interface, VDU Dispatcher.
//      Date :      13th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

static void _VDUSwitchMode(uint32_t newMode);

/*
        This table is the number of additional bytes needed for each VDU command
*/

const uint8_t _VDUCommandLengths[32] = {
        0,                                                                          // 0 does nothing
        1,                                                                          // 1 sends character to the printer
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                                              // 2-16 are all single byte VDU commands
        1,                                                                          // 17 sets text colour (COLOUR)
        2,                                                                          // 18 sets graphic colour (GCOL)
        5,                                                                          // 19 redefines colour
        0,0,                                                                        // 20,21 single byte
        1,                                                                          // 22 sets the mode (MODE)
        9,                                                                          // 23 defines UDG & controls 6845
        8,                                                                          // 24 sets the graphics window
        5,                                                                          // 25 is PLOT 
        0,0,                                                                        // 26,27 are single byte
        4,                                                                          // 28 sets the text window
        4,                                                                          // 29 sets the graphics origin
        0,                                                                          // 30 is single byte
        2                                                                           // 31 is move text cursor
};

static uint8_t _vduBuffer[16];                                                      // Buffered input to VDU command
static uint8_t _vduRequired = 0;                                                    // Amount of data required
static uint8_t _vduIndex = 0;                                                       // Current index into _vduBuffer
static uint8_t _vduPendingCommand = 0;                                              // Command to do when all collected.
static bool writeTextToGraphics = false;                                            // When set, text output is via graphics
static bool vduEnabled = true;                                                      // Text I/O enabled ?

/**
 * @brief      Initialise the VDU Graphic subsystem
 */
void VDUInitialise(void) {
    static bool isInitialised = false;
    if (isInitialised) return;
    isInitialised = true;
    DVIInitialise();                                                                // Initialise DVI
    VDUFontInitialise();                                                            // Copy default font to UDG
    VDUWrite(22);                                                                   // Switch mode.    
    VDUWrite(MODE_640_480_8);
}

/**
 * @brief      Extract signed 16 bit integer from the VDU Buffer
 *
 * @param[in]  ofst  Offset of LSB in VDU Buffeer
 *
 * @return     Signed 16 bit value.
 */
int _VDUShort(int ofst) {
    int r = _vduBuffer[ofst] + (_vduBuffer[ofst+1] << 8);                           // Value as UInt
    return ((r & 0x8000) == 0) ? r : r - 0x10000;                                   // Return as signed integer
}

/**
* @brief      Write one character or control code (bodge version)
*
* @param[in]  c     Character code
*/

void VDUWrite(uint8_t c) {
    int x1,y1,x2,y2;

    if (DVIGetModeInformation() == NULL) return;                                    // Check screen is actually on.

    if (_vduRequired == 0) {                                                        // New command ?
        _vduPendingCommand = c;                                                     // The pending command.
        _vduRequired = (c < 32) ? _VDUCommandLengths[c] : 0;                        // How much to grab before we do it ?
        _vduIndex = 0;                                                              // Reset the index.
    } else {
        _vduBuffer[_vduIndex++] = c;                                                // Collecting data, save the data.
        _vduRequired--;                                                             // One fewer required
    }

    if (_vduRequired != 0) return;                                                  // We still want more.

    if (!vduEnabled) {                                                              // If VDU is disabled.
        if (_vduPendingCommand != 1 && _vduPendingCommand != 6) return;             // Exit for everything except 1 and 6
    }

    switch(_vduPendingCommand) {                                                    // So, what do we do as we now have a complete command.

        case 0:                                                                     // 0 does nothing.
            break;

        case 1:                                                                     // 1 outputs to printer, which we don't have, so does nothing.
        case 2:                                                                     // 2 & 3 sets the printer off and on ... same problem.
        case 3:
            break;

        case 4:                                                                     // 4 sets text mode
            writeTextToGraphics = false;
            break;

        case 5:                                                                     // 5 sets graphic mode
            VDUHideCursor();                                                        // Not visible in this mode.
            writeTextToGraphics = true;                                             
            break;

        case 6:                                                                     // 6 re-enables VDU
            vduEnabled = true;
            break;  

        case 7:                                                                     // 7 is the beep, which is not supported.
            break;

        case 8:                                                                     // 8..11 are cursor moves
        case 9:
        case 10:
        case 11:
        case 13:                                                                    // 13 New line ... out of order.
            VDUHideCursor();
            if (writeTextToGraphics) {
                VDUGCursor(c);
            } else {
                VDUCursor(c);
            }
            break;

        case 12:                                                                    // 12 Clear Screen (CLS)
            VDUHideCursor();
            VDUClearScreen();     
            VDUHomeCursor();
            break;

        case 14:                                                                    // 14,15 control paged mode, not supported
        case 15:
            break;

        case 16:                                                                    // 16 clears graphics (CLG)
            VDUHideCursor();
            VDUClearGraphicsWindow();
            break;

        case 17:                                                                    // 17 sets text colour (COLOUR)
            VDUSetTextColour(_vduBuffer[0]);
            break;

        case 18:                                                                    // 18 sets graphics mode & colour (GCOL)
            VDUSetGraphicsColour(_vduBuffer[0],_vduBuffer[1]);                       
            break;

        case 19:                                                                    // 19 colour redefine (not allowed)            
            break;

        case 20:                                                                    // 20 set default text, graphics colours (and mapping)
            VDUSetDefaultTextColour();
            VDUSetDefaultGraphicColour();
            break;

        case 21:
            VDUHideCursor();
            vduEnabled = false;                                                     // 21 stops all text and graphic output.
            break;

        case 22:                                                                    // 22 n Change mode (MODE)
            VDUHideCursor();
            _VDUSwitchMode(_vduBuffer[0]);                                                  VDUEnableCursor();
    
            break;

        case 23:                                                                    // 23 c <data> Define user defined graphic
            switch(_vduBuffer[0]) {
                case 1:
                    if (_vduBuffer[1])
                        VDUEnableCursor();
                    else
                        VDUDisableCursor();
                break;
   	        case 7:
		    VDUHideCursor();
		    VDUScrollRect(_vduBuffer[1],_vduBuffer[2]);
	        break;
            default:
                VDUDefineCharacter(_vduBuffer[0],_vduBuffer+1);
                break;
            }
            break;
        case 24:                                                                    // 24 define graphics window
            x1 = _VDUShort(0);y1 = _VDUShort(2);                                    // Get coordinates.
            x2 = _VDUShort(4);y2 = _VDUShort(6);
            VDUSetGraphicsWindow(min(x1,x2),min(y1,y2),max(x1,x2),max(y1,y2));      // Set the window.
            break;

        case 25:                                                                    // 25 Plot cmd,x,y command PLOT
            VDUHideCursor();
            VDUPlot(_vduBuffer[0],_VDUShort(1),_VDUShort(3));
            break;

        case 26:                                                                    // 26 reset text and graphics windows
            VDUHideCursor();
            VDUResetTextWindow();
            VDUResetGraphicsWindow();
            VDUSetGraphicsOrigin(0,0);
            VDUSetTextCursor(0,0);
            VDUResetGraphicsCursor();
            break;

        case 27:                                                                    // 27 does nothing  
            break;

        case 28:                                                                    // 28 set text window
            VDUHideCursor();
            VDUSetTextWindow(min(_vduBuffer[0],_vduBuffer[2]),
                             max(_vduBuffer[1],_vduBuffer[3]),
                             max(_vduBuffer[0],_vduBuffer[2]),
                             min(_vduBuffer[1],_vduBuffer[3]));
            break;

        case 29:                                                                    // 29 set graphics origin
            VDUSetGraphicsOrigin(_VDUShort(0),_VDUShort(2));
            break;

        case 30:                                                                    // 30 is Home cursor
            VDUHideCursor();
            VDUHomeCursor();
            break;

        case 31:                                                                    // 31 is position cursor
            VDUHideCursor();
            VDUSetTextCursor(_vduBuffer[0],_vduBuffer[1]);
            break;

        case 127:                                                                   // 127 is destructive backspace
            VDUWrite(8);VDUWrite(' ');VDUWrite(8);
            break;
                        
        default:            
            VDUHideCursor();
            if (c >= ' ') {                                                         // Legit character
                if (writeTextToGraphics) {                                          // This is VDU 5 mode.
                    VDUGWriteText(c);
                } else {
                    VDUWriteText(c);                                                // Output character if legitimate and enabled.
                }
            }
            break;
    }
    if (!writeTextToGraphics) VDUShowCursor();
}

/**
 * @brief      Write a 16 bit value to the VDU Stream
 *
 * @param[in]  word  Word to write
 */
void VDUWriteWord(uint32_t word) {
    VDUWrite(word & 0xFF);
    VDUWrite((word >> 8) & 0xFF);
}

/**
 * @brief      Change the display mode
 *
 * @param[in]  newMode  Mode to change it to ; invalid values are ignored.
 */
static void _VDUSwitchMode(uint32_t newMode) {
    if (newMode < 0 || newMode >= DVI_MODE_COUNT) return;                           // Validate the mode.
    DVISetMode(newMode);                                                            // Set the physical driver mode.
    VDUWrite(20);                                                                   // Reset colours
    VDUWrite(26);                                                                   // Reset windows and origin
    VDUWrite(12);                                                                   // Clear the screen
    VDUWrite(30);                                                                   // Home cursor
    VDUWrite(6);                                                                    // Enable text/graphics output
    VDUWrite(4);                                                                    // Enable text mode
}

/**
 * @brief      Support function for VDUWrite which allows printf
 *
 * @param[in]  fmt        format string
 * @param[in]  <unnamed>  Assorted parameters.
 */

void VDUWriteString(char *fmt, ...) {
    char buf[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, 128, fmt, args);
    uint8_t *p = (uint8_t *)buf;
    while (*p != '\0') VDUWrite((*p++) & 0xFF);
    va_end(args);
}

