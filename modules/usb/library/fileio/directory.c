// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      directory.c
//      Purpose :   Directory functions
//      Date :      5th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "usb_module.h"
#include "usb_module_local.h"

/**
 * @brief      Open a directory for reading
 *
 * @param      dirName  Directory name to open
 *
 * @return     Error code (-ve) Directory Handle (+ve, 0)
 */
uint32_t FSOpenDirectory(char *dirName) {
    CHECKFSAVAILABLE();                                                             // Storage available ?
    DIR *pDir;
    if (!FSProcessFileName(&dirName)) return FSERR_BADNAME;                         // Validate name
    uint32_t newHandle = FSAllocateRecord(true);                                     // Allocate directory record
    if (newHandle < 0) return newHandle;                                            // Failed for some reason (probably too many open)
    uint32_t err = FSGetValidateHandle(newHandle,true,(void **)&pDir);               // Validate the handle and get the directory object.
    if (err != 0) LOG("Failure !!");
    FRESULT fr = f_opendir(pDir,dirName);                                           // Open for reading.
    if (fr != FR_OK) {                                                              // Did it fail ?
        FSFreeRecord(newHandle);                                                    // If so don't need the new record
        return FSMapErrorCode(fr);                                                  // Return error code.
    }
    return newHandle;
}

/**
 * @brief      Read the next directory record. Note this is not sorted.
 *
 * @param[in]  handle  Directory handle
 * @param      fso     Pointer to an information structure
 *
 * @return     0 or rerror code.
 */
uint32_t FSReadDirectory(uint32_t handle,FSOBJECTINFO *fso) {
    CHECKFSAVAILABLE();                                                             // Storage available ?
    DIR *pDir;
    FILINFO fInfo;
    uint32_t err = FSGetValidateHandle(handle,true,(void **)&pDir);                  // Validate the handle and get the directory object.
    if (err != 0) return err;
    FRESULT res = f_readdir(pDir,&fInfo);                                           // Read next element.
    if (fInfo.fname[0] == 0) return FSERR_EOF;                                      // End of directory read, return EOF.
    if (res != FR_OK) return FSMapErrorCode(res);                                   // All the other error codes.
    strcpy(fso->name,fInfo.fname);                                                  // Fill in the structure, name first (need to improve this)
    fso->isDirectory = (fInfo.fattrib & AM_DIR) != 0;                               // Directory flag.
    fso->size = fInfo.fsize;                                                        // Size of file.
    return 0;
}

/**
 * @brief      Close directory read
 *
 * @param[in]  handle  File System Object handle
 *
 * @return     0 or error code.
 */
uint32_t FSCloseDirectory(uint32_t handle) {
    DIR *pDir;
    uint32_t err = FSGetValidateHandle(handle,true,(void **)&pDir);                  // Validate the handle and get the directory object.
    if (err != 0) return err;
    FSFreeRecord(handle);                                                           // Free up the handle.
    return FSMapErrorCode(f_closedir(pDir));                                        // Close it and return mapped error code.
}

