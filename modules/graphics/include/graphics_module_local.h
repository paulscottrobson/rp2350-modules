

#pragma once
#ifndef RUNTIME
#endif

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

struct _GraphicWindow {
    int xLeft,yBottom,xRight,yTop;
};

extern struct _GraphicWindow window;

void VDUCursor(int c);
void VDUWriteText(uint8_t c);
void VDUClearScreen(void);
void VDUSetTextCursor(int x,int y);
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

void VDUAUp(void);
void VDUADown(void);
void VDUALeft(void);
void VDUARight(void);

int  VDUAReadPixel(int x,int y);
void VDUHorizontallFill(int x,int y,bool rightOnly);
int VDUGetBackgroundColour(void);
void VDUGCursor(int c);
void VDUGWriteText(int c);  
void VDUAOutputByte(int x,int y,uint8_t pixelData);
void VDUHideCursor(void);
void VDUShowCursor(void);
void VDUEnableCursor(void);
void VDUDisableCursor(void);

#define GFXC_NOENDPOINT     (0x01)
#define GFXC_DOTTED         (0x02)
