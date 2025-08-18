// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Machine wrapper test app
//      Date :      17th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "machine_module.h"

/**
 * @brief      The standard machine start.
 *
 * @param[in]  argc  arguments used
 * @param      argv  argument values.
 *
 * @return     { description_of_the_return_value }
 */
int MAINPROGRAM(int argc,char *argv[]) {
    MACInitialise();                                                                // Set up
    //
    //      Register modules to check the commands here. 
    //
    MACAddCommandHandler(MACLogCommands);                                           // Log commands doesn't consume.
    //
    MACAddCommandHandler(MACOSCommand);                                             // Handle the i/o commands etc. Should probably be last !
    MACStart();                                                                     // And run 
    return 0;    
}

/**
 * @brief      A user provided boot screen text
 */
void MACBootDisplay(void) {
    VDUWrite(17);VDUWrite(3);
    VDUWriteString("*** The test computer system ***\r\n\n");
    MACSetStandardColour();
    VDUWriteString("Platform              %s\r\n",COMPlatformName());
    VDUWriteString("Static RAM Available  %dk\r\n",MEMGetSRAMSize()/1024);
    VDUWriteString("Pseudo SRAM Available %dk\r\n\n",PSRGetMemorySize()/1024);
}