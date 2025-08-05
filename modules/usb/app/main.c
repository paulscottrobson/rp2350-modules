// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Test program for USB Interface
//      Date :      5th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "common_module.h"
#include "usb_module.h"

static void ListDirectory(void);
static void ListFile(void);

/**
 * @brief      Handle USB Report
 *
 * @param      r     USB Report
 *
 * @return     false, as the report is not consumed.
 */
bool _ReportHandler(USBREPORT *r) {
    char buffer[128];
    sprintf(buffer,"%d %04x:%04x (%2d)",r->type,r->vid,r->pid,r->length);
    for (int i = 0;i < r->length;i++) sprintf(buffer+strlen(buffer)," %02x",r->data[i]);
    LOG(buffer);
    return false;
}

/**
 * @brief      Example main program
 *
 * @return     Error code
 */
int MAINPROGRAM() {
    USBInitialise();                                                                // Set up
    USBInstallHandler(_ReportHandler);                                              // Add a handler for USB HID reports.
    LOG("Installed.");
    ListDirectory();                                                                // List the directory
    ListFile();                                                                     // List the file.
    LOG("Free memory %d",COMGetFreeSystemMemory());
    while (COMAppRunning()) {                                                       // Run USB dumping USB reports as raw data
        COMUpdate();
    }
}


/**
 * @brief      List the root directory
 */
static void ListDirectory(void) {
    char *path = ""; 
    int32_t error,handle = FSOpenDirectory(path);
    if (handle >= 0) {
        FSOBJECTINFO fInfo;
        while (error = FSReadDirectory(handle,&fInfo),error == 0) {
            LOG("%c %-8d %s",fInfo.isDirectory ? 'D':'.',fInfo.size,fInfo.name);
        }
        if (error != FSERR_EOF) LOG("Read error : %d",error);
        FSCloseDirectory(handle);        
    }
}

/**
 * @brief      List part of a file on the USB key.
 */
static void ListFile(void) {
    int32_t error,handle = FSOpen("loops.bsc");
    if (handle == 0) {
        error = FSSeek(handle,12);
        LOG("Seek result %d",error);
        LOG("Tell result %d",FSTell(handle));
        char buffer[129];
        error = FSRead(handle,buffer,128);buffer[128] = '\0';
        LOG("Read %d : [%s]",error,buffer);
        error = FSClose(handle);
    }
}

