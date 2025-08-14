// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      markers.c
//      Purpose :   Markers required for the screen editor
//      Date :      14th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//                  Lennart Benschop
//
// *******************************************************************************************
// *******************************************************************************************
 
#include "graphics_module.h"
#include "graphics_module_local.h"

void VDUResetTextEndMarkers(void) {
    LOG("M:Reset");
}
void VDUScrollTextEndMarkers(int dir) {
    LOG("M:Scroll %d",dir);
}

void VDUSetTextEndMarker(int y) {
    LOG("M:Mark %d",y);
}

bool VDUHasLineEndMarker(int y) {
    return false;
}
