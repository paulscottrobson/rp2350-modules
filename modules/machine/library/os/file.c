// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      file.c
//      Purpose :   OS Commands (file)
//      Date :      18th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "machine_module.h"
#include "machine_module_local.h"           

/**
 * @brief      Copy File
 *
 * @param[in]  argc  argument count
 * @param      argv  argument array
 *
 * @return     zero or error code.
 */

int MACOSCopyFile(int argc,char **argv) {
    int32_t err = FSCreate(argv[2]);                                                // Create second file
    if (err < 0) return err;

    int32_t inFile = FSOpen(argv[1]);                                               // Open first file
    if (inFile < 0) return inFile;

    int32_t outFile = FSOpen(argv[2]);                                              // Open second file.
    if (outFile < 0) {
        FSClose(inFile); 
        return outFile;
    }
    uint8_t buffer[128];                                                            // Buffered copy.
    int32_t count;
    while (count = FSRead(inFile,buffer,sizeof(buffer)), count > 0) {               // While more to write
        int32_t written = FSWrite(outFile,buffer,count);
        if (written < 0 || written != count) {                                      // Write failed for some reason.
            FSClose(inFile);FSClose(outFile);
            return FSERR_SYSTEM;
        }
    }
    FSClose(inFile);                                                                // Close files and exit.
    FSClose(outFile);
    return 0;
}

/**
 * @brief      Delete a file.
 *
 * @param[in]  argc  argument count
 * @param      argv  argument array
 *
 * @return     zero or error code.
 */

int MACOSDeleteFile(int argc,char **argv) {
    return FSDelete(argv[1]);
}

