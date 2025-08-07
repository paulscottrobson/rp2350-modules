// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      directory.c
//      Purpose :   Directory functions
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "runtime.h"
#include "usb_module.h"
#include "sys/stat.h"
#include "dirent.h"
#include "errno.h"

/**
 * @brief      Open a directory for reading
 *
 * @param      dirName  Directory name to open
 *
 * @return     Error code (-ve) Directory Handle (+ve, 0)
 */
uint32_t FSOpenDirectory(char *dirName) {
    CHECKFSAVAILABLE();                                                             // Storage available ?
    DIR **ppDir;
    if (!FSProcessFileName(&dirName)) return FSERR_BADNAME;                         // Validate name
    uint32_t newHandle = FSAllocateRecord(true);                                     // Allocate directory record
    if (newHandle < 0) return newHandle;                                            // Failed for some reason (probably too many open)
    uint32_t err = FSGetValidateHandle(newHandle,true,(void **)&ppDir);              // Validate the handle and get the directory object.
    if (err != 0) LOG("Failure !!");
    *ppDir = opendir(dirName);                                                      // Open for reading.
    if (*ppDir == NULL) {                                                           // Did it fail ?
        FSFreeRecord(newHandle);                                                    // If so don't need the new record
        return FSMapErrorCode();                                                    // Return error code.
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
    DIR **ppDir;
    uint32_t err = FSGetValidateHandle(handle,true,(void **)&ppDir);                 // Validate the handle and get the directory object.
    if (err != 0) return err;
    struct dirent *next = readdir(*ppDir);                                          // Read next entry.
    if (next == NULL) {                                                             // Read failed.
        if (errno == 0 || errno == EAGAIN) return FSERR_EOF;                        // End of directory
        return FSMapErrorCode();                                                    // All other errors.
    }
    strcpy(fso->name,next->d_name);
    fso->isDirectory = (next->d_type == DT_DIR);                                    // Directory flag.
    fso->size = fso->isDirectory ? 0:1;                                             // Size of file (dummy)
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
    DIR **ppDir;
    uint32_t err = FSGetValidateHandle(handle,true,(void **)&ppDir);                     // Validate the handle and get the directory object.
    if (err != 0) return err;
    FSFreeRecord(handle);                                                               // Free up the handle.
    closedir(*ppDir);
    return FSMapErrorCode();                                                            // Close it and return mapped error code.
}

