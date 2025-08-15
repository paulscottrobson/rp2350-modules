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


#define MAX_HEIGHT      (60)

#define SAVED_COORDS    (3)

struct VDUConfig {
    int xCursor,yCursor;                                                            // Posiiton in character cells in the window.
    VDUWINDOW tw;                                                                   // Text window
    int fgCol,bgCol;                                                                // Foreground & Background colour
    int textWidth,textHeight;                                                       // Width/Height of text character
    int xOrigin,yOrigin;                                                            // Origin position, this is in logical coordinates.
    int gColMode,fgrGraphic,bgrGraphic;                                             // Graphic mode, foreground, background graphic colours
    VDUWINDOW gw;                                                                   // Graphic window
    int xLogicalExtent,yLogicalExtent;                                              // The extent of the logical coordinates.
    int xScale,yScale;                                                              // Scale (divide by this for logical -> physical)
    int xCoord[SAVED_COORDS],yCoord[SAVED_COORDS];                                  // Coordinate buffer (PHYSICAL coordinates)
    int xLastLogical,yLastLogical;                                                  // Last logical
    bool writeTextToGraphics;                                                       // When set, text output is via graphics
    bool vduEnabled;                                                                // Text I/O enabled ?
    uint8_t udgMemory[128*8];                                                       // Memory for user defined graphics.
    bool isExtendedLine[MAX_HEIGHT];                                                // Extended line flag for each line.
    bool cursorIsVisible;                                                           // Cursor visible
    bool cursorIsEnabled;                                                           // Cursor is enabled.
};

extern struct VDUConfig vc;

void VDUCursor(int c);
void VDUSetTextCursor(uint8_t x,uint8_t y);
void VDUWriteText(uint8_t c);
void VDUClearScreen(void);
void VDUHomeCursor(void);

void VDUResetGraphicsCursor(void);
void VDUSetGraphicsOrigin(int x,int y);
uint8_t VDUGetCharacterLineData(int c,int y,bool largeFont);
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

int  VDUAReadPixel(int x,int y,bool isValid);
void VDUHorizontallFill(int x,int y,bool rightOnly);
int  VDUGetBackgroundColour(void);
void VDUGCursor(int c);
void VDUGWriteText(int c);  
void VDUAOutputByte(int x,int y,uint8_t pixelData);
void VDUHideCursor(void);
void VDUShowCursor(void);
void VDUEnableCursor(void);
void VDUDisableCursor(void);

void VDUResetTextEndMarkers(void);                                                      
void VDUScrollTextEndMarkers(int dir);
void VDUSetTextEndMarker(int y);


#define GFXC_NOENDPOINT     (0x01)
#define GFXC_DOTTED         (0x02)
