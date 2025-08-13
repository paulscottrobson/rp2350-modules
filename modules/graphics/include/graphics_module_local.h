// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      graphics_module_local.h
//      Purpose :   Internal headers.
//      Date :      13th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************
 
#pragma once
#ifndef RUNTIME
#endif

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

struct _Window {
    int xLeft,yBottom,xRight,yTop;
};

#define SAVED_COORDS    (3)

struct VDUConfig {
    int xCursor,yCursor;                                                            // Posiiton in character cells in the window.
    struct _Window tw;                                                              // Text window
    int fgCol,bgCol;                                                                // Foreground & Background colour
    int  textHeight;                                                                // Height of text character
    int xOrigin,yOrigin;                                                            // Origin position, this is in logical coordinates.
    int gColMode,fgrGraphic,bgrGraphic;                                             // Graphic mode, foreground, background graphic colours
    struct _Window gw;                                                              // Graphic window
    int xCoord[SAVED_COORDS],yCoord[SAVED_COORDS];                                  // Coordinate buffer (PHYSICAL coordinates)
    bool writeTextToGraphics;                                                       // When set, text output is via graphics
    bool vduEnabled;                                                                // Text I/O enabled ?
    uint8_t udgMemory[128*8];                                                       // Memory for user defined graphics.
};

extern struct VDUConfig vc;

void VDUCursor(int c);
void VDUWriteText(uint8_t c);
void VDUClearScreen(void);
void VDUHomeCursor(void);

void VDUResetGraphicsCursor(void);
void VDUSetGraphicsOrigin(int x,int y);
uint8_t VDUGetCharacterLineData(int c,int y);
void VDUDefineCharacter(int c,uint8_t *gData);
void VDUSetDrawingData(int drawMode);

void VDUSetDefaultTextColour(void);
void VDUSetTextColour(int colour);
void VDUResetTextWindow(void);   
void VDUSetTextWindow(int x1,int y1,int x2,int y2);
void VDUSetGraphicsWindow(int x1,int y1,int x2,int y2);
void VDUSetDefaultGraphicColour(void);
void VDUResetGraphicsWindow(void);
void VDUPlotDispatch(int cmd,int *xCoord,int *yCoord);                                           
void VDUClearGraphicsWindow(void);
void VDUSetTextSize(uint8_t xSize,uint8_t ySize);

void VDUASetActionColour(int act,int col);
void VDUASetControlBits(int c);
void VDUAPlot(int x,int y);
void VDUAHorizLine(int x1,int x2,int y);
void VDUAVertLine(int x,int y1,int y2);
void VDUALine(int x0, int y0, int x1, int y1);
void VDUAFillRect(int x0,int y0,int x1,int y1);
void VDUAFrameRect(int x0,int y0,int x1,int y1);
void VDUAFillEllipse(int x0,int y0,int x1,int y1);
void VDUAFrameEllipse(int x0,int y0,int x1,int y1);
void VDUAFillTriangle(int x0,int y0,int x1,int y1,int x2,int y2);
void VDUAFrameTriangle(int x0,int y0,int x1,int y1,int x2,int y2);
void VDUFontInitialise(void);

void VDUAUp(void);
void VDUADown(void);
void VDUALeft(void);
void VDUARight(void);

int  VDUAReadPixel(int x,int y);
void VDUHorizontallFill(int x,int y,bool rightOnly);
int  VDUGetBackgroundColour(void);
void VDUGCursor(int c);
void VDUGWriteText(int c);  
void VDUAOutputByte(int x,int y,uint8_t pixelData);
void VDUHideCursor(void);
void VDUShowCursor(void);
void VDUEnableCursor(void);
void VDUDisableCursor(void);

#define GFXC_NOENDPOINT     (0x01)
#define GFXC_DOTTED         (0x02)
