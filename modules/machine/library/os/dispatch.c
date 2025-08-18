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
 * @brief      List of commands, parameters, and function to call
 */
static struct _CommandList {
    char *command;
    int  paramsRequired;
    MACCOMMANDHANDLER commandFunc;
} commandList[] = {
        { "ls",0,       MACOSListDirectory } ,
        { "dir",0,      MACOSListDirectory } ,
        { "cd",1,       MACOSChangeDirectory } ,
        { "chdir",1,    MACOSChangeDirectory } ,
        { "rd",1,       MACOSRemoveDirectory } ,
        { "rmdir",1,    MACOSRemoveDirectory } ,
        { "md",1,       MACOSCreateDirectory } ,
        { "mkdir",1,    MACOSCreateDirectory } ,
        { "pwd",1,      MACOSPrintDirectory } ,
        { "type",-1,    MACOSTextDump } ,
        { "cat",-1,     MACOSTextDump } ,
        { "hd",-1,      MACOSHexDump },
        { "hexdump",-1, MACOSHexDump },
        { "copy",2,     MACOSCopyFile } ,
        { "cp",2,       MACOSCopyFile } ,
        { "rm",1,       MACOSDeleteFile } ,
        { "del",1,      MACOSDeleteFile } ,
        { "help",0,     MACOSListCommands },
        { NULL,0, NULL },
};

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

/**
 * @brief      List all OS commands that are recognised.
 *
 * @param[in]  argc  argument count
 * @param      argv  argument array
 *
 * @return     zero or error code.
 */
int MACOSListCommands(int argc,char **argv) {
    return 0;
}

/**
 * @brief      Compare 2 strings case insensitive
 *
 * @param      s1    first string
 * @param      s2    second string
 *
 * @return     true if they are the same.
 */
bool MACOSCompare(char *s1,char *s2) {
    while (tolower(*s1) == tolower(*s2)) {
        if (*s1 == '\0') return true;
        s1++;s2++;
    }
    return false;
}