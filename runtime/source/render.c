// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      render.c
//      Purpose :   Graphic display renderer.
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include <runtime.h>

uint8_t framebuf[VIDEO_BYTES];                                                      // Bitplane memory

//
//      Palettes for 8 colour and 64 colour mode
//
static int palette_8[8] = { 0x000,0xF00,0x0F0,0xFF0,0x00F,0xF0F,0x0FF,0xFFF };

static int palette_64[64] = {
    0x000, 0x500, 0xA00, 0xF00,
    0x050, 0x550, 0xA50, 0xF50,
    0x0A0, 0x5A0, 0xAA0, 0xFA0,
    0x0F0, 0x5F0, 0xAF0, 0xFF0,

    0x005, 0x505, 0xA05, 0xF05,
    0x055, 0x555, 0xA55, 0xF55,
    0x0A5, 0x5A5, 0xAA5, 0xFA5,
    0x0F5, 0x5F5, 0xAF5, 0xFF5,

    0x00A, 0x50A, 0xA0A, 0xF0A,
    0x05A, 0x55A, 0xA5A, 0xF5A,
    0x0AA, 0x5AA, 0xAAA, 0xFAA,
    0x0FA, 0x5FA, 0xAFA, 0xFFA,
    
    0x00F, 0x50F, 0xA0F, 0xF0F,
    0x05F, 0x55F, 0xA5F, 0xF5F,
    0x0AF, 0x5AF, 0xAAF, 0xFAF,
    0x0FF, 0x5FF, 0xAFF, 0xFFF,
};
/**
 * @brief      Initialise the rendering system. This means creating working
 *             palettes ready to go for the different renderer. The mono palette
 *             can do 4 colour Greyscale and Monochrome
 */
void DVIInitialise(void) { 
    for (int i = 0;i < VIDEO_BYTES;i++) framebuf[i] = rand();   
}


/**
 * @brief      Render the display
 *
 * @param      surface  The surface to render it on
 */
void RNDRender(SDL_Surface *surface) {  
    DVIMODEINFO *dm = DVIGetModeInformation();
    uint8_t *pr,*pg,*pb,r,g,b;
    SDL_Rect rc;
    rc.x = rc.y = 0;rc.w = AS_SCALEX*640+16;rc.h = AS_SCALEY*480+16;
    SYSRectangle(&rc,0);
    for (int y = 0;y < dm->height;y++) {
        rc.w = AS_SCALEX * 640/dm->width;
        rc.h = AS_SCALEY * 480/dm->height;
        rc.y = y*rc.h+8;
        pr = dm->bitPlane[0]+y*dm->bytesPerLine;
        pg = dm->bitPlane[1]+y*dm->bytesPerLine;
        pb = dm->bitPlane[2]+y*dm->bytesPerLine;
        for (int x = 0;x < dm->width;x+= 8/dm->bitPlaneDepth) {
            rc.x = x*rc.w+8;
            r = *pr++;g = *pg++;b = *pb++;
            if (dm->bitPlaneDepth == 1) {
                for (int bt = 0;bt < 8;bt++) {
                    uint8_t c = ((r & 0x80) >> 7)+((g & 0x80) >> 6)+((b & 0x80) >> 5);
                    SYSRectangle(&rc,palette_8[c]);
                    r <<= 1;g <<= 1;b <<= 1;
                    rc.x += rc.w;
                }
            } else {
                for (int bt = 0;bt < 8;bt++) {
                    uint8_t c = ((r & 0xC0) >> 6)+((g & 0xC0) >> 4)+((b & 0xC0) >> 2);
                    SYSRectangle(&rc,palette_64[c]);
                    r <<= 2;g <<= 2;b <<= 2;
                    rc.x += rc.w;         
                }       
            }
        }
    } 
}

