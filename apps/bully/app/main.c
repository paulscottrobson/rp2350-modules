// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Bully allows us to bully the libraries and check for stability.
//      Date :      19th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "bully_module.h"

static void ListDirectory(void);
static void ListFile(void);
static void decorate(void);

/**
 * @brief      Test for vsync callback.
 */
static void speckleTest(void) {
    uint8_t *vRAM = DVIGetModeInformation()->bitPlane[0];
    for (int i = 0;i < 100;i++) {
        vRAM[random() % (320*10)+320*10] = random();
    }
}

int MAINPROGRAM(int argc,char *argv[]) {
    INPInitialise();                                                                // Initialise input
    INPSetFunctionKey(2,"ABCD");
    VDUInitialise();
    VDUWrite(22);VDUWrite(MODE_320_240_8);
    //VDUWrite(1);VDUWrite(1);VDUWrite(2);                                          // Double height
    decorate();                                                                     // Draw ellipses.
    VDUWrite(28);VDUWrite(2);VDUWrite(18);VDUWrite(30);VDUWrite(5);

    while (COMAppRunning()) {                                                       // Our "main program"
        int16_t k = INPGetKey();                                                    // Get key, log to serial and list if F or D pressed
        if (k != 0) {
            LOG("Key %d",k);
            VDUWrite(k);
        }
        if (toupper(k) == 'F') ListFile();
        if (toupper(k) == 'D') ListDirectory();
        if (toupper(k) == 'S') {                                                    // Show free memory
            LOG("Free memory %d",COMGetFreeSystemMemory());
        }
        if (toupper(k) == 'M') {                                                    // M switches mode so I can test that.
            VDUWrite(22);VDUWrite(MODE_320_240_64);
            decorate();
        }
        speckleTest();
        COMUpdate();                                                                // Update stuff.
    }
}

/**
 * @brief      Draw rectangles so we've something to see.
 */
static void decorate(void) {
    for (int i = 0; i < 400;i += 12) {                                              // Draw *something* as a background :)
        VDUSetGraphicsColour(0,rand() % 7+1);                                      
        VDUPlot(4,i,i);
        VDUPlot(5,1279-i,i);VDUPlot(5,1279-i,959-i);
        VDUPlot(5,i,959-i);VDUPlot(5,i,i);
    }
    // for (int i = 0;i < 8;i++) {
    //     GFXDraw(RawColour,i,i);
    //     GFXDraw(Move,i*10,100);GFXDraw(FillRect,i*10+9,139);
    // }
}

/**
 * @brief      List the root directory
 */
static void ListDirectory(void) {
    char *path = "/cdtest/..";
    LOG(path);
    FSChangeDirectory(path);
    path = ""; 
    int32_t error,handle = FSOpenDirectory(path);
    if (handle >= 0) {
        FSOBJECTINFO fInfo;
        while (error = FSReadDirectory(handle,&fInfo),error == 0) {
            LOG("%c %-8d %s",fInfo.isDirectory ? 'D':'.',fInfo.size,fInfo.name);
        }
        if (error != FSERR_EOF) LOG("Read error : %d",error);
        FSCloseDirectory(handle);        
        LOG("..."); 
    } else {
        LOG("Couldn't open path %s",path);
    }
}

/**
 * @brief      List part of a file on the USB key.
 */
static void ListFile(void) {
    FSChangeDirectory("/cdtest");
    int32_t error,handle = FSOpen("test.txt");
    if (handle == 0) {
        error = FSSeek(handle,12);
        char buffer[129];
        error = FSRead(handle,buffer,36);buffer[36] = '\0';
        LOG("Read %d : [%s]",error,buffer);
        error = FSClose(handle);
    }
}
