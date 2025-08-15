// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      windows.c
//      Purpose :   Window functions.
//      Date :      14th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

/**
 * @brief      Reset the text window
 */
void VDUResetTextWindow(void) {    
    DVIMODEINFO *dmi = DVIGetModeInformation();            
    vc.tw.xLeft = vc.tw.yTop = 0;
    vc.tw.xRight = (dmi->width / 8)-1;
    vc.tw.yBottom = (dmi->height / vc.textHeight) - 1;
    VDUResetTextEndMarkers();                                                       // Reset the text end markers.
}

/**
 * @brief      Get the text window
 *
 * @return     Pointer to window structure
 */
VDUWINDOW *VDUGetTextWindow(void) {
    return &vc.tw;
}

/**
 * @brief      Set the text window (cells are inclusive)
 *
 * @param[in]  x1    x Left
 * @param[in]  y1    y Bottom
 * @param[in]  x2    x Right
 * @param[in]  y2    y Top
 */
void VDUSetTextWindow(int x1,int y1,int x2,int y2) {
    DVIMODEINFO *dmi = DVIGetModeInformation();            
    int w = (dmi->width / 8)-1;
    int h = (dmi->height / vc.textHeight)-1;
    vc.tw.xLeft = x1;vc.tw.yTop = y2;
    vc.tw.xRight = min(w,x2);vc.tw.yBottom = min(h,y1);
    VDUResetTextEndMarkers();                                                       // Reset the text end markers.    
}

/**
 * @brief      Reset the graphics window
 */
void VDUResetGraphicsWindow(void) {
    VDUSetGraphicsWindow(0,0,9999,9999);                                            // This works because VDUSetGraphicsWindow clips.
}

/**
 * @brief      Set Graphics Window
 *
 * @param[in]  x1    Left X Logical
 * @param[in]  y1    Bottom Y Logical
 * @param[in]  x2    Right X Logical
 * @param[in]  y2    Top Y Logical
 */
void VDUSetGraphicsWindow(int x1,int y1,int x2,int y2) {
    DVIMODEINFO *dmi = DVIGetModeInformation();                                     // Get the display information.

    vc.xScale = (dmi->width == 640) ? 1 : 2;                                        // Currently we only have 640 and 320 across.
    vc.yScale = (dmi->height == 480) ? 1 : 2;                                       // Support 480,240 and 256 down.

    vc.xLogicalExtent = (dmi->width << vc.xScale)-1;                                // Work out the logical extent.   
    vc.yLogicalExtent = (dmi->height << vc.yScale)-1;

    x1 = max(0,x1);y1 = max(0,y1);                                                  // Clip window coordinates to logical extent.
    x2 = min(x2,vc.xLogicalExtent);y2 = min(y2,vc.yLogicalExtent);

    x1 = x1 >> vc.xScale;y1 = y1 >> vc.yScale;                                      // Convert to physical coordinates 
    x2 = x2 >> vc.xScale;y2 = y2 >> vc.yScale;      

    vc.gw.xLeft = x1;vc.gw.yBottom = y1;                                            // Set the window
    vc.gw.xRight = x2;vc.gw.yTop = y2;                                  

    //printf("Window:%d,%d - %d,%d\n",vc.gw.xLeft,vc.gw.yBottom,vc.gw.xRight,vc.gw.yTop);
}

/**
 * @brief      Clear the text screen to the background, inside the text window.
 */
void VDUClearScreen(void) {    
    for (int x = vc.tw.xLeft;x <= vc.tw.xRight;x++) {                               // Probably quick enough....
        for (int y = vc.tw.yTop;y <= vc.tw.yBottom;y++) {
            VDURenderCharacter(x,y,' ');
        }
    }
    VDUResetTextEndMarkers();                                                       // Reset the text end markers.
}

/**
 * @brief      Clear the current graphics window (e.g. CLG)
 */
void VDUClearGraphicsWindow(void) {
    VDUASetActionColour(0,vc.bgrGraphic);                                           // Background colour,no tweaks.
    VDUASetControlBits(0);
    VDUAFillRect(vc.gw.xLeft,vc.gw.yBottom,vc.gw.xRight,vc.gw.yTop);                // Fill the window
}
