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

/**
 * @brief      Initialise the rendering system. This means creating working
 *             palettes ready to go for the different renderer. The mono palette
 *             can do 4 colour Greyscale and Monochrome
 */
void DVIInitialise(void) {    
}


/**
 * @brief      Render the display
 *
 * @param      surface  The surface to render it on
 */
void RNDRender(SDL_Surface *surface) {  
}

