# Machine Module

## Required Modules 

- common
- usb
- input
- dvi
- graphics
- screeneditor
- memory
- psram

## Purpose

This provides a base machine which can be converted to a specific machine.

## Further Modules

None planned.

## Usage Notes

The idea is explored best via the demo machine : machine/app/main.c ; this is a whole new machine.

It is initialised by MACInitialise() and started by MACStart(). The user provides a MACBootDisplay() function which shows the boot display.

When started it allows repeated entry of lines using the screen editor. These are handled by the command handler. There are two here. MACOSCommand provides the OS commands - these are things that allow very simple manipulation of the file system, function keys etc.

The second is more typical and shows how a command handler works ; it is in machine/library/setup.c at the bottom.

As with C apps, an argument array and argument count are provided, which contain the parsed OS commands. 

The MAC_PROVIDES macro provides a command list to the help system so you could have MAC_PROVIDES("open close jump swing") for example.

The argv == NULL check is required.

The rest of the example just dumps the arguments to the log . The function returns 0 (not processed) or 1 (processed). It is not processed here, but if a command is recognised and acted on it should return 1, so the command is not passed on to other handlers.

## OS Commands

These are much simpler than a command shell, there are no wild cards or similar. Spaces can be included using quote marks (either single or double)

| Command            | Purpose                                                      |
| ------------------ | ------------------------------------------------------------ |
| ls                 | List current directory (also dir)                            |
| cd [directory]     | Change directory (also chdir)                                |
| rd [directory]     | Remove directory if it exists (also rmdir)                   |
| md [directory]     | Create a new directory (also mkdir)                          |
| pwd                | Display current directory                                    |
| hd [file]          | Dump a file in hex / character format (also hexdump)         |
| cat [file]         | Dump a file in character format, does its best with 13/10 codes (also type) |
| cp [file1] [file2] | Copies file1 to file2 (also copy)                            |
| rm [file1]         | Deletes file 1 (also del)                                    |
| fkey [key] [text]  | Define a function key e.g. fkey 4 "hello world". Note that the bar syntax allows control codes - \|c is ASCII code for c (67,$43) anded with $1F, e.g. 3 |




## Revision

Written by Paul Robson, last revised 12st August 2025.





