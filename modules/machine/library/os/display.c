// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      display.c
//      Purpose :   OS Commands (file display)
//      Date :      18th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "machine_module.h"
#include "machine_module_local.h"           

/**
 * @brief      Dump as text
 *
 * @param[in]  argc  argument count
 * @param      argv  argument array
 *
 * @return     zero or error code.
 */

#define ISCRLF(c)   ((c) == 13 || (c) == 10)

int MACOSTextDump(int argc,char **argv) {
    int32_t count;
    int32_t inFile = FSOpen(argv[1]);                                               // Open file
    if (inFile < 0) return inFile;
    uint8_t ch,lastCharacter = 0;
    while (count = FSRead(inFile,&ch,1),count == 1) {
        if (ISCRLF(ch) && !ISCRLF(lastCharacter)) {
            VDUWrite(13);VDUWrite(10);
        } else {
            VDUWrite(ch);
        }
        lastCharacter = ch;
    }
    FSClose(inFile);
    return count;
}

/**
 * @brief      Dump in hexadecimal
 *
 * @param[in]  argc  argument count
 * @param      argv  argument array
 *
 * @return     zero or error code.
 */

int MACOSHexDump(int argc,char **argv) {
    int32_t count,offset = 0;
    int32_t lineSize = (DVIGetModeInformation()->width > 320) ? 16 : 8;
    int32_t inFile = FSOpen(argv[1]);                                               // Open file
    if (inFile < 0) return inFile;
    uint8_t buffer[16];                                                             // The current 8 or 16 bytes
    char temp[3],text[81];
    while (count = FSRead(inFile,buffer,lineSize),count > 0) {
        sprintf(text,"%06x%70s",(int)offset,"");                                    // Initial line with just address
        text[(lineSize == 16) ? 74 : 40] = '\0';                                    // Truncate it
        for (int i = 0;i <  count;i++) {
            uint8_t c = buffer[i];
            sprintf(temp,"%02x",c);
            text[8+i*3] = temp[0];
            text[9+i*3] = temp[1];
            if (c < ' ' || c == 0x7F) c = '.';
            text[((lineSize == 16) ? 57:32)+i] = c;
        }
        offset += count;
        VDUWriteString("%s\r\n",text);
    }
    FSClose(inFile);
    return count;
}

