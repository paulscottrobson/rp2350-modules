// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      setup.c
//      Purpose :   Machine wrapper setup
//      Date :      17th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "machine_module.h"
#include "machine_module_local.h"           

MACINFO mcInfo;

char *arguments[MAX_ELEMENTS];                                                      // Array of element pointers.
MACCOMMANDHANDLER handlers[MAX_COMMAND_LISTENERS];                                  // Array of command listeners.

/**
 * @brief      Initialise the machine.
 */
void MACInitialise(void) {
    static bool isInitialised = false;
    if (isInitialised) return;
    isInitialised = true;

    MEMInitialise();
    INPInitialise();
    VDUInitialise();
    SEDInitialise();

    //
    //      Initialise machine information structure.
    //
    mcInfo.inkColour = 6;
    mcInfo.paperColour = 4;
    mcInfo.errorColour = 1;
    mcInfo.outputColour = 7;
    mcInfo.mode = MODE_640_480_8;
    mcInfo.doubleHeight = true;
    mcInfo.buffer = NULL;
    mcInfo.bufferSize = 256;
    mcInfo.argumentCount = 0;
    mcInfo.arguments = arguments;
    mcInfo.handlerCount = 0;
    mcInfo.commandHandlers = handlers;
}

/**
 * @brief      Add a command handler
 *
 * @param      handler  The command handler.
 */
void MACAddCommandHandler(MACCOMMANDHANDLER handler) {
    if (mcInfo.handlerCount < MAX_COMMAND_LISTENERS) {
        mcInfo.commandHandlers[mcInfo.handlerCount++] = handler;
    } else {
        ERROR("Too many command handlers.");
    }
}

/**
 * @brief      Non cancelling command logger
 *
 * @param[in]  argc  argument count
 * @param      argv  arguments
 *
 * @return     false as it never consumes the arguments. 0 if argv is NULL.
 */
int MACLogCommands(int argc,char **argv) {
    if (argv == NULL) return 0;                                                     // Required.
    LOG("Command Line Log : Total %d",argc);
    for (int i = 0;i < argc;i++) {
        LOG("%d [%s]",i,argv[i]);
    }         
    return 0;
}
