#include "dvi_module.h"

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "dvi_config.h"
#include "dvi_common.h"

int main() {
    DVIStart();
    DVISetMode(DVI_MODE_320_240_8);
    DVISetMode(DVI_MODE_320_256_8);
    DVISetMode(DVI_MODE_640_480_2);
    DVISetMode(DVI_MODE_320_240_64);
    DVISetMode(DVI_MODE_640_480_8);

    while (1) {}
}
