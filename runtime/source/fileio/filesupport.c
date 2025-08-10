// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      filesupport.c
//      Purpose :   File I/O Support routines
//      Date :      7th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "usb_module.h"
#include "sys/stat.h"
#include "errno.h"
#include "dirent.h"

#define MAXFILESDIRS    (8)                                                         // Max # of files/directories open.

//
//      Each represents a file or directory.
//
struct _filedirRecord {
    enum { Unused, File, Directory } type;                                          // What each record holds.
    union {
        FILE *file;                                                                 // FatFS file structure
        DIR  *directory;                                                            // Directory object structure.
    } ff;
} fsObject[MAXFILESDIRS];

/**
 * @brief      Initialise the file system wrapper.
 */
void FSInitialise(void) {
    for (int i = 0;i < MAXFILESDIRS;i++) fsObject[i].type = Unused;                 // Set all types to unused.
    FSChangeDirectory("/");                                                         // Change directory to the root.
}

/**
 * @brief      Validate a handle and convert it to the appropriate form (DIR or
 *             FIL) as a void.
 *
 * @param[in]  handle       File or Directory Handle
 * @param[in]  isDirectory  if true, expecting a directory, if false, expecting
 *                          a file.
 * @param      fsObjectPtr  pointer to a pointer to store the FIL or DIR
 *                          reference.
 *
 * @return     Error Code or 0
 */
uint32_t FSGetValidateHandle(uint32_t handle, bool isDirectory,void **fsObjectPtr) {
    if (handle < 0 || handle >= MAXFILESDIRS) return FSERR_BADHANDLE;               // Handle out of range.
    if (fsObject[handle].type == Unused) return FSERR_BADHANDLE;                    // Not actually in use
    if (fsObject[handle].type != (isDirectory ? Directory:File)) return FSERR_TYPE; // Wrong type of handle.
    *fsObjectPtr = (void *)&(fsObject[handle].ff);                                  // Return pointer to file/directory part.
    return 0;
}

/**
 * @brief      Map a FATFS Error code onto a USB Module one.
 *
 * @return     Error code (or 0 if no error)
 */
uint32_t FSMapErrorCode(void) {
    uint32_t e;
    switch(errno) {
        case 0:
            e = 0;break;
        case ENOENT:
            e = FSERR_EXIST;break;
        default:
            e = FSERR_SYSTEM;break;
    }
    return e;
}

/**
 * @brief      Preprocess a file or directory name
 *
 * @param      pFileName  pointer to the file name string.
 *
 * @return     true if valid.
 */
static char buffer1[128],buffer2[128];
static int buffID = 0;

bool FSProcessFileName(char **pFileName) {
    buffID = 1-buffID;                                                              // 2 alternating buffers, some functs take 2 parameters.
    char *buffer = buffID ? buffer1:buffer2;
    strcpy(buffer,"storage/");strcat(buffer,*pFileName);
    *pFileName = buffer;
    return true;
}

/**
 * @brief      Allocate a record to be used to hold a file or directory object
 *
 * @param[in]  isDirectory  true for directory, false for file
 *
 * @return     handle of the record to used or -1 if none available.
 */
uint32_t FSAllocateRecord(bool isDirectory) {
    for (uint32_t i = 0;i < MAXFILESDIRS;i++) {
        if (fsObject[i].type == Unused) {
            fsObject[i].type = isDirectory ? Directory : File;
            return i;
        }
    }
    return -1;
}

/**
 * @brief      Free a record in the file/directory structure.
 *
 * @param[in]  handle  The handle
 */
void FSFreeRecord(uint32_t handle) {
    fsObject[handle].type = Unused;
}
