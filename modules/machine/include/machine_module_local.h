// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      machine_module_local.h
//      Purpose :   Machine headers (internal)
//      Date :      17th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#pragma once
#ifndef RUNTIME
#endif

#define MAX_INPUT_SIZE      (256)                                                   // Max size of one input line.
#define MAX_ELEMENTS        (8)                                                     // Maximum number of elements in one input line (command & parameters)

#define MAX_COMMAND_LISTENERS (8)                                                   // Max number of command listeners.

int MACOSChangeDirectory(int argc,char **argv);
int MACOSRemoveDirectory(int argc,char **argv);
int MACOSCreateDirectory(int argc,char **argv);
int MACOSPrintDirectory(int argc,char **argv);
int MACOSTextDump(int argc,char **argv);
int MACOSHexDump(int argc,char **argv);
int MACOSListDirectory(int argc,char **argv);
int MACOSCopyFile(int argc,char **argv);
int MACOSDeleteFile(int argc,char **argv);
int MACRequestCommandWords(int argc,char **argv);

bool MACOSCompare(char *s1,char *s2);
void MACOSListCommands(void);