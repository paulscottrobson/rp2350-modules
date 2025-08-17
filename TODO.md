# TODO list

## Machine module
- Boot header (could really be a subclass, but still avoiding C++)
- Generic input handler, error reporting, split input into argc/argv structure.
- Modules can register to receive commands,
- * command handler, exists as callable seperately and a handler. 
- background updatable module (no they can register using COMAddUpdateFunction() )

## Sprite module
- Complete reimplementation, possibly two, either xor/palette or double buffer/draw ?

## Audio ?

## Others
- python script with skeleton for runtimes CMakeLists.txt
- add a library to an app/module instructions/script
- builder to allow libraries that aren't in the dependency list (e.g. input and usb in console, for testing keyboard.)

