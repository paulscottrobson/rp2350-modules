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
static uint32_t FSChangeDirectorySingle(char *change);

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
    char changeDir[MAXDIRSIZE+1];
    uint32_t cd = 0;
    if (*newDir == '/') {                                                           // Absolute path, so back to the root
        strcpy(current,"/");
    }
    while (*newDir == '/') newDir++;                                                // Skip any leading slashes.

    while (*newDir != '\0') {                                                       // Finished each directory change ?
        cd = 0;
        while (*newDir != '\0' && *newDir != '/') {                                 // While not read in complete subsection 
            if (cd == MAXDIRSIZE) return FSERR_BADNAME;                             // Name is too long.
            changeDir[cd++] = *newDir++;                                            // Copy it in.
        }
        changeDir[cd] = '\0';                                                       // Make ASCIIZ
        LOG("Before %s %s",current,changeDir);
        uint32_t err = FSChangeDirectorySingle(changeDir);
        LOG("After %s",current);
        if (err != 0) return err;
        while (*newDir == '/') newDir++;                                            // Skip any leading slashes.
    }
    return 0;
}

/**
 * @brief      Make a single change to the current directory.
 *
 * @param      change  Change : new directory, ..  or .
 *
 * @return     Error code.
 */
static uint32_t FSChangeDirectorySingle(char *change) {
    if (strcmp(change,".") == 0) return 0;                                          // . doesn't change anything at all.

    if (strcmp(change,"..") == 0) {                                                 // .. goes up one level.
        if (strlen(current) == 1) return FSERR_EXIST;                               // Already at the top.
    }
    //
    //      Change to a named directory.
    //
    strcat(current,change);
    return 0;
}