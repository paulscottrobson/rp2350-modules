// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      dispatch.c
//      Purpose :   OS Commands setup
//      Date :      18th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "machine_module.h"
#include "machine_module_local.h"           

/**
 * @brief      Execute an OS command string
 *
 * @param      cmd   Command string
 *
 * @return     0 or error code.
 */
int MACOSCommandString(char *cmd) {
    char *arguments[MAX_ELEMENTS];
    uint8_t count = MACParseInput(cmd,arguments,MAX_ELEMENTS);                      // Parse arguments out
    return MACOSCommand(count,arguments);                                           // Pass to the executor.
}

/**
 * @brief      Perform an OS command
 *
 * @param[in]  argc  argument count
 * @param      argv  argument array
 *
 * @return     zero or error code.
 */
int MACOSCommand(int argc,char **argv) {
    if (argc == 0 || argv == NULL) return 0;                                        // Nothing to do.
    LOG(argv[0]);
    return 0;
}

