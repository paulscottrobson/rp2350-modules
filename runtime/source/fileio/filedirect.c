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
int32_t FSCreate(char *fileName) {
    CHECKFSAVAILABLE();                                                             // Storage available ?
    FILE *fil;
    if (!FSProcessFileName(&fileName)) return FSERR_BADNAME;
    unlink(fileName);                                                               // Delete any existing file.
    fil = fopen(fileName,"w");                                                      // Try to create it.
    if (fil == NULL) return FSMapErrorCode();                                       // We couldn't for some reason.
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
int32_t FSDelete(char *fileName) {
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
int32_t FSCreateDirectory(char *dirName) {
    CHECKFSAVAILABLE();                                                             // Storage available ?
    if (!FSProcessFileName(&dirName)) return FSERR_BADNAME;
    mkdir(dirName,0755);

    #ifdef RUNTIME
    if (errno == EAGAIN) return 0;                                                  // I have no idea why EAGAIN is returned here and I can't find anything online !
    #endif                                                                          // This only seems to happen when it's a new directory.

    if (errno == EEXIST || errno == 0) return 0;                                    // Ignore exist errors.
    return FSMapErrorCode();
}

/**
 * @brief      Delete a directory if it exists
 *
 * @param      dirName  Directory to delete
 *
 * @return     error code or 0.
 */
int32_t FSDeleteDirectory(char *dirName) {
    CHECKFSAVAILABLE();                                                             // Storage available ?
    if (!FSProcessFileName(&dirName)) return FSERR_BADNAME;
    rmdir(dirName);
    return 0;
}

