// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      common.c
//      Purpose :   Common code
//      Date :      5th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "common_module.h"
#include "common_module_local.h"

static COMUPDATEFUNCTION updateFunctions[MAX_UPDATE_FUNCS];                         // Update functions
static uint32_t updateFunctionCount = 0;
/**
 * @brief      Common initialise code.
 */
void COMInitialise(void) {
    static bool isInitialised = false;                                              // Only initialise once.
    if (isInitialised) return;
    isInitialised = true;
    updateFunctionCount = 0;                                                        // No defined update functions
    stdio_init_all();                                                               // Initialise the serial port to stdio
}

/**
 * @brief      This allows the runtime version to stop, when false it will exit.
 *             In the Pico, this is an infinite loop
 *
 * @return     true if running.
 */
bool COMAppRunning(void) {
    return true;                                                                    // So we are always running.
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
    printf(buf);printf("\n");
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
}

/**
 * @brief      Return the system clock
 *
 * @return     Elapsed time in milliseconds
 */
uint32_t COMClock(void) {
    return (uint32_t)(time_us_64() >> 10);
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
 * @brief      Update all registered updateable modules.
 */
void COMUpdate(void) {
    for (int i = 0;i < updateFunctionCount;i++) {
        (*updateFunctions[i])();
    }
}

/**
 * @brief      Get an approximation of the amount of free system memory.
 *
 * @return     Free memory in bytes.
 */
uint32_t COMGetFreeSystemMemory(void) {
   extern char __StackLimit, __bss_end__;   
   return &__StackLimit  - &__bss_end__;
}