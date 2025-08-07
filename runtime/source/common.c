// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      common.c
//      Purpose :   Common code
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include <runtime.h>

#define MAX_UPDATE_FUNCS    (8)

static COMUPDATEFUNCTION updateFunctions[MAX_UPDATE_FUNCS];                         // Update functions
static uint32_t updateFunctionCount = 0;

/**
 * @brief      Dummy initialise
 */
void COMInitialise(void) {
}

/**
 * @brief      Get elapsed time since start
 *
 * @return     time in 1khz ticks
 */
uint32_t COMClock(void) {
    return SDL_GetTicks();
}

/**
 * @brief      Delay for a given time period, updating in the background.
 *
 * @param[in]  ms    The milliseconds
 */
void COMWait(uint32_t ms) {
    uint32_t endTime = COMClock() + ms;
    while (COMClock() < endTime && COMAppRunning()) {
        COMUpdate();
    }
}

/**
 * @brief      Register an update function
 *
 * @param[in]  updateFunc  Update function to register
 */
void COMAddUpdateFunction(COMUPDATEFUNCTION updateFunc) {
    if (updateFunctionCount < MAX_UPDATE_FUNCS) {
        updateFunctions[updateFunctionCount++] = updateFunc;
    }
}

/**
 * @brief      Update the various systems, and Yield for SDL.
 */
void COMUpdate(void) {
    for (int i = 0;i < updateFunctionCount;i++) {
        (*updateFunctions[i])();
    }
    SYSYield();
}

/**
 * @brief      Print string on debug log.
 *
 * @param      format     Format string
 * @param[in]  <unnamed>  Data for format string.
 */
void COMPrint(char *format,...) {
    char buf[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, 128, format, args);
    printf("%s\n",buf);
    va_end(args);
}

/**
 * @brief      Report an error
 *
 * @param      msg       Error message
 * @param[in]  line      Line number
 * @param      fileName  Source file
 */
void COMError(char *msg,int line,char *fileName) {
    COMPrint("[ERROR] %s (%s:%d)",msg,fileName,line);
    exit(-1);
}

/**
 * @brief      Return the free system memory. This is a dummy, obviously. If
 *             using the Video it probably needs to be higher
 *
 * @return     Free bytes of memory.
 */
uint32_t COMGetFreeSystemMemory(void) {
    return 262144;
}
