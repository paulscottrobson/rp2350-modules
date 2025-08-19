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
        { "pwd",0,      MACOSPrintDirectory } ,
        { "type",1,     MACOSTextDump } ,
        { "cat",1,      MACOSTextDump } ,
        { "hd",1,       MACOSHexDump },
        { "hexdump",1,  MACOSHexDump },
        { "copy",2,     MACOSCopyFile } ,
        { "cp",2,       MACOSCopyFile } ,
        { "rm",1,       MACOSDeleteFile } ,
        { "del",1,      MACOSDeleteFile } ,
        { "help",0,     MACRequestCommandWords },
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
    if (argc == -1) {
        MACOSListCommands();
        return 0;
    }
    if (argc == 0 || argv == NULL) return 0;                                        // Nothing to do.
    //LOG(argv[0]);
    VDUWrite(17);VDUWrite(mcInfo.outputColour);
    int i = 0;
    while (commandList[i].command != NULL) {                                        // Scan through looking for match.
        if (MACOSCompare(commandList[i].command,argv[0])) {
            int pCount = commandList[i].paramsRequired;                             // How many parameters ?
            if (pCount >= 0 && argc != pCount + 1) {                                // Either variable (-1) or a specific number of parameters.
                MACError("Wrong number of parameters");
                return 1;
            }            
            int error = (*commandList[i].commandFunc)(argc,argv);                   // Go do it.
            if (error != 0) {                                                       // Error occurred.
                char errorMsg[16];
                sprintf(errorMsg,"Error %d",error);
                MACError(errorMsg);
            }
            return 1;
        }
        i++;
    }
    return 0;
}

/**
 * @brief      List all OS commands that are recognised.
 *
 */
void MACOSListCommands(void) {
    int i = 0;
    while (commandList[i].command != NULL) {
        VDUWriteString("%s ",commandList[i++].command);
    }
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