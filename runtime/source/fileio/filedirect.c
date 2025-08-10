// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      filedirect.c
//      Purpose :   Operations on complete files/directories (e.g. no open/close)
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "runtime.h"
#include "usb_module.h"

/**
 * @brief      Create a new file, deleting the old one if it exists.
 *
 * @param      fileName  File Name to create
 *
 * @return     error code or 0
 */
uint32_t FSCreate(char *fileName) {
    CHECKFSAVAILABLE();                                                             // Storage available ?
    FILE *fil;
    if (!FSProcessFileName(&fileName)) return FSERR_BADNAME;
    FSDelete(fileName);                                                             // Delete any existing file.
    fil = fopen(fileName,"w");                                                      // Try to create it.
    if (fil != NULL) return FSMapErrorCode();                                       // We couldn't for some reason.
    fclose(fil);                                                                    // Close it straight away.
    return 0;
}

/**
 * @brief      Delete a file if it exists.
 *             
 * @param      fileName  File Name to delete
 *
 * @return     0 
 */
uint32_t FSDelete(char *fileName) {
    CHECKFSAVAILABLE();                                                             // Storage available ?
    if (!FSProcessFileName(&fileName)) return FSERR_BADNAME;
    unlink(fileName);
    return 0;
}

/**
 * @brief      Create a directory, ignored if it already exists.
 *
 * @param      dirName  Directory to create
 *
 * @return     error code or 0.
 */
uint32_t FSCreateDirectory(char *dirName) {
    CHECKFSAVAILABLE();                                                             // Storage available ?
    if (!FSProcessFileName(&dirName)) return FSERR_BADNAME;
    mkdir(dirName,0777);
    if (errno == EEXIST) return 0;                                                  // Ignore exist errors.
    return FSMapErrorCode();
}

/**
 * @brief      Change directory
 *
 * @param      dirName  The directory to change to.
 *
 * @return     Error code or zero.
 */
uint32_t FSChangeDirectory(char *dirName) {
    CHECKFSAVAILABLE();
    if (!FSProcessFileName(&dirName)) return FSERR_BADNAME;
    if (chdir(dirName) < 0) return FSMapErrorCode();                                // Change directory
    return 0;
}


/**
 * @brief      Delete a directory if it exists
 *
 * @param      dirName  Directory to delete
 *
 * @return     error code or 0.
 */
uint32_t FSDeleteDirectory(char *dirName) {
    CHECKFSAVAILABLE();                                                             // Storage available ?
    if (!FSProcessFileName(&dirName)) return FSERR_BADNAME;
    rmdir(dirName);
    return 0;
}

