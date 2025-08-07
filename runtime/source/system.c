// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      system.c
//      Purpose :   System code
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include <runtime.h>

static SDL_Window *mainWindow = NULL;
static SDL_Surface *mainSurface = NULL;

static int startTime = 0,endTime = 0,frameCount = 0;

#define RED(x) ((((x) >> 8) & 0xF) * 17)
#define GREEN(x) ((((x) >> 4) & 0xF) * 17)
#define BLUE(x) ((((x) >> 0) & 0xF) * 17)

/**
 * @brief      Open the main window and start everything off
 *
 * @param[in]  muteSound  Mute sound on simulator
 *
 */
void SYSOpen(bool muteSound) {

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_GAMECONTROLLER) < 0)    {   // Try to initialise SDL Video and Audio
        exit(printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError()));
    }
    mainWindow = SDL_CreateWindow("RP2350PC Runtime System",                        // Try to create a window
                            SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, 
                            640*AS_SCALEX+16,480*AS_SCALEY+16, SDL_WINDOW_SHOWN );
    if (mainWindow == NULL) {
        exit(printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() ));
    }

    mainSurface = SDL_GetWindowSurface(mainWindow);                                 // Get a surface to draw on.

    // CTLFindControllers();                                                        // Have to be done after SDL Initialisation.
    // SOUNDOpen();
    // SNDMuteAllChannels();                                                        // Mute all channels
    // if (!muteSound) SOUNDPlay();

    SDL_ShowCursor(SDL_DISABLE);                                                    // Hide mouse cursor
    startTime = COMClock();
}


static int isRunning = -1;                                                          // Is app running

/**
 * @brief      Check the SDL2 message queue, update mouse and keyboard, update display.
 *
 * @return     { description_of_the_return_value }
 */
int SYSPollUpdate(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {                                                 // While events in event queue.
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {     // Exit if ESC/Ctrl+ESC pressed tbc,
            int ctrl = ((SDL_GetModState() & KMOD_LCTRL) != 0);                     // If control pressed
            if (ctrl == 0) isRunning = 0;                                           // Exit
        }
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {                 // Handle other keys, which may go up or down.
            KBDProcessEvent(event.key.keysym.scancode,SDL_GetModState(),event.type == SDL_KEYDOWN);
        }
        if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN      // Mouse button/position update
                                                        || event.type == SDL_MOUSEBUTTONUP) {
            SYSUpdateMouse();
        }
        if (event.type == SDL_MOUSEWHEEL) {                                         // Handle scroll wheel events.
            // int dy = event.wheel.y;
            // if (event.wheel.type == SDL_MOUSEWHEEL_FLIPPED) dy = -dy;
            // MSEUpdateScrollWheel(dy);
        }
        if (event.type == SDL_QUIT) {                                               // Exit on Alt+F4 etc.
            isRunning = 0;
        }
    }
    frameCount++;
    SYSRectangle(NULL,0x008);
    RNDRender(mainSurface);
    SDL_UpdateWindowSurface(mainWindow);                                            // And update the main window.  
    return isRunning;
}

/**
 * @brief      Close down everything
 */
void SYSClose(void) {
    endTime = COMClock();
    SDL_DestroyWindow(mainWindow);                                                  // Destroy working window
    // SOUNDStop();
    // SDL_CloseAudio();                                                            // Shut audio up.
    SDL_Quit();                                                                     // Exit SDL.
    printf("Frame Rate %.2f\n",frameCount/((endTime-startTime)/1000.0));
}


/**
 * @brief      Return the drawing surface
 *
 * @return     SDL Surface pointer
 */
SDL_Surface *SYSGetSurface(void) {
    return mainSurface;
}

/**
 * @brief      Draw arectangle on the working surface
 *
 * @param      rc      Rectangle dimensions
 * @param[in]  colour  Fill Colour
 */
void SYSRectangle(SDL_Rect *rc,int colour) {
    SDL_FillRect(mainSurface,rc,SDL_MapRGB(mainSurface->format,RED(colour),GREEN(colour),BLUE(colour)));
}


