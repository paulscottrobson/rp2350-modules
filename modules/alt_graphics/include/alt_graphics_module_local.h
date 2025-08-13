// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      alt_graphics_module_local.h
//      Purpose :   Graphics module local header.
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************
 
#pragma once
#ifndef RUNTIME
#endif

//
//      This represents the current pixel drawing state.
//
#define MAX_CONTEXTS    (4)                                                         // Max size of context stack.

typedef struct _clipping {
    int32_t     xLeft,yTop,xRight,yBottom;                                          // Clipping rectangle for drawing (inclusive)
} GFXCLIPRECT;

struct DrawingContext {
    uint32_t    currentMode;                                                        // Current mode set up for.
    GFXCLIPRECT clip;                                                               // Window clipping
    GFXMAPPER   mapper;                                                             // Mapper function (can be NULL)
    GFXFONTSOURCEFUNCTION font;                                                     // Current function for getting font information.
    uint32_t    x,y;                                                                // Current position.
    int32_t     xPrev[3],yPrev[3];                                                  // Up to 3 previous coordinates (physical)

    uint8_t     foreground,background;                                              // Colour pixels (already masked correctly for mode) in LSB positions.
    bool        isTransparent;                                                      // True when transparent background.
    uint8_t     drawMode;                                                           // Drawing mode.
    uint8_t     xFontScale,yFontScale;                                              // Font scalars.
};

extern struct DrawingContext *draw;                                                 // Current draw information.
extern int contextStackPointer;
extern uint32_t currentMode;
extern DVIMODEINFO modeInfo;
extern bool inDrawingVert,inDrawingHoriz;              

#define CHECKUPDATE()               GFXCheckModeChange()
#define SORT_PAIR(c1,c2)            if (c1 > c2) { int32_t t = c1;c1 = c2;c2 = t; }

void GFXPreProcess(int32_t *x,int32_t *y);

void GFXRawMove(int32_t x,int32_t y);
void GFXRawPlot(bool useFgr);
void GFXRawUp(void);
void GFXRawDown(void);
void GFXRawLeft(void);    
void GFXRawRight(void);
void GFXRawWordRight(uint32_t colour);

void GFXCheckModeChange(void);
void GFXInitialiseDrawStructure(void);
void GFXResetClipping(void);
uint32_t GFXGetCharacterExtent(uint32_t code);

void GFXDrawDesktop(void);
void GFXDrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, bool drawLastPixel);
void GFXDrawRectangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1,bool fill);
void GFXDrawEllipse(int32_t x0, int32_t y0, int32_t x1, int32_t y1,bool fill);
void GFXDrawFilledTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1,int32_t x2,int32_t y2);
void GFXDrawOutlineTriangle(int32_t x0,int32_t y0,int32_t x1,int32_t y1,int32_t x2,int32_t y2);
uint32_t GFXDrawCharacter(uint32_t x,uint32_t y,uint32_t code);
void GFXOptimisedHorizontalLine(int32_t x0, int32_t x1, int32_t y,bool useFgr);


