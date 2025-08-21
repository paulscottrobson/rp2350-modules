// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      functionkey.c
//      Purpose :   OS Commands (function key)
//      Date :      21st August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "machine_module.h"
#include "machine_module_local.h"           

/**
 * @brief      Define a function key
 *
 * @param[in]  argc  argument count
 * @param      argv  argument array
 *
 * @return     zero or error code.
 */

int MACOSDefineFunctionKey(int argc,char **argv) {
    int key = atoi(argv[1]);                                                        // Function key number
    if (key < 1 || key >= MAC_FUNCKEYCOUNT) return FSERR_BADNAME;                   // Keys numbered 1-12 
    if (strlen(argv[2]) > MAC_FUNCKEYTEXTSIZE) return FSERR_STORAGE;                // Name too long.
    strcpy(mcInfo.keyDefs[key-1],argv[2]);                                          // Put in static storage.
    INPSetFunctionKey(key,mcInfo.keyDefs[key-1]);                                   // Notify input system
    return 0;
}

