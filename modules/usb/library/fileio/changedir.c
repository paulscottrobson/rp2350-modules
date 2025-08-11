// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      changedir.c
//      Purpose :   Change directory code.
//      Date :      11th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "usb_module.h"

#define MAXDIRSIZE      (64)
#define MAXFILESIZE     (32)

static char current[MAXDIRSIZE+1];                                                  // The current directory.

/**
 * @brief      Initialise the CWD system
 */
void FSCDInitialise(void) {
    strcpy(current,"/");                                                            // Back to the root directory.
}

/**
 * @brief      Given a file or directory name, which may be absolute or
 *             relative, convert to absolute
 *
 * @param      name  The file Name (max 32 characters)
 *
 * @return     Processed file name.
 */
char *FSCDMapCurrentName(char *name) {
    static char mapBuffer[MAXDIRSIZE+MAXFILESIZE+3];                                // Buffer for the final file name.
    if (strlen(name) > MAXFILESIZE) ERROR("Filename too long");                     // It's a retrocomputer, we aren't having massive file names.
    if (*name == '/') return name;                                                  // The name is already an absolute path as it starts with /
    sprintf(mapBuffer,"%s/%s",current,name);                                        // Build the name from the CWD and the stub.
    return mapBuffer;
}

/**
 * @brief      Change the current working directory
 *
 * @param      newDir  Directory, which could be absolute or relative
 *
 * @return     Error codes.
 */
uint32_t FSChangeDirectory(char *newDir) {
    if (*newDir == '/') {                                                           // Absolute path, so back to the root
        strcpy(current,"/");
        newDir++;                                                                   // Consume initial slash.
    }
    //
    //      Scan through / seperated entries which can be a directory .. . or empty (cwd ////) and make and
    //      validate those changes.
    //
    strcat(current,newDir);
    return 0;
}