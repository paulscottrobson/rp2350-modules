#include "common_module.h"
#include "dvi_module.h"

int MAINPROGRAM(int argc,char *argv[]) {
    DVIInitialise();
 //   DVISetMode(MODE_320_240_8);
 //   DVISetMode(MODE_320_256_8);
 //   DVISetMode(MODE_320_240_64);
 //   DVISetMode(MODE_640_480_8);
 
    while (COMAppRunning()) {    
        COMUpdate();
    }
}