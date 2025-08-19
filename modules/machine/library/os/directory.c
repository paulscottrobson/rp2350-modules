// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      directory.c
//      Purpose :   OS Commands (directory)
//      Date :      18th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "machine_module.h"
#include "machine_module_local.h"           

/**
 * @brief      List Directory
 *
 * @param[in]  argc  argument count
 * @param      argv  argument array
 *
 * @return     zero or error code.
 */

int MACOSListDirectory(int argc,char **argv) {
    int32_t error = 0,handle = FSOpenDirectory("");
    if (handle >= 0) {
        FSOBJECTINFO fInfo;
        while (error = FSReadDirectory(handle,&fInfo),error == 0) {
            if (!fInfo.isDirectory) {
                VDUWriteString("%-32s %d\r\n",fInfo.name,fInfo.size);
            } else {
                VDUWriteString("%-32s (directory)\r\n",fInfo.name,fInfo.size);
            }
        }
        if (error == FSERR_EOF) error = 0;
        FSCloseDirectory(handle);        
        LOG("..."); 
    } else {
        error = handle;
    }

    return error;
}

/**
 * @brief      Change current directory
 *
 * @param[in]  argc  argument count
 * @param      argv  argument array
 *
 * @return     zero or error code.
 */

int MACOSChangeDirectory(int argc,char **argv) {
    return FSChangeDirectory(argv[1]);
}

/**
 * @brief      Remove directory
 *
 * @param[in]  argc  argument count
 * @param      argv  argument array
 *
 * @return     zero or error code.
 */

int MACOSRemoveDirectory(int argc,char **argv) {
    return FSDeleteDirectory(argv[1]);
}

/**
 * @brief      Create directory
 *
 * @param[in]  argc  argument count
 * @param      argv  argument array
 *
 * @return     zero or error code.
 */

int MACOSCreateDirectory(int argc,char **argv) {
    return FSCreateDirectory(argv[1]);
}

/**
 * @brief      Print the current directory
 *
 * @param[in]  argc  argument count
 * @param      argv  argument array
 *
 * @return     zero or error code.
 */

int MACOSPrintDirectory(int argc,char **argv) {
    VDUWriteString("%s\r\n",FSGetCurrentDirectory());
    return 0;
}
