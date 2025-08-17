// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      parse.c
//      Purpose :   Parse Input
//      Date :      17th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "machine_module.h"
#include "machine_module_local.h"           

static char *MACExtractArgument(char *p);

#define ISQUOTEDCHAR(c)   ((c) == '"' || (c) == '\'')

/**
 * @brief      Parse the input buffer to the output arguments.
 *
 * @param      p            pointer to the input line, ASCIIZ
 * @param      arguments    arguments array to fill
 * @param[in]  argumentMax  maximum number of arguments allowed.
 *
 * @return     { description_of_the_return_value }
 */
uint8_t MACParseInput(char *p,char **arguments,int argumentMax) {
    uint8_t argumentCount = 0;                                                      // Erase the argument array.
    while (isspace(*p)) p++;
    while (*p != '\0' && argumentCount != argumentMax) {                            // While more arguments.
        bool isQuotedString = ISQUOTEDCHAR(*p); 
        arguments[argumentCount++] = isQuotedString ? p+1:p;                        // Store reference to arguments, skipping the first quoted character.
        p = MACExtractArgument(p);                                                  // Skip to end of this argument, e.g. the byte after.
        if (*p != '\0') {                                                           // If not already at end
            *p++ = '\0';                                                            // Truncate argument here.
            while (isspace(*p)) p++;                                                // Skip spaces.
        }
    }
    return argumentCount;
}

/**
 * @brief      Extract next argument. Handle single/double quoted arguments,
 *             remove the trailing quote
 *
 * @param      p     Start position in string
 *
 * @return     End position in string.
 */
static char *MACExtractArgument(char *p) {
    if (ISQUOTEDCHAR(*p)) {                                                         // Quoted string
        char match = *p++;      
        while (*p != match && *p != '\0') p++;                                      // Skip to match or string end.
    } else {                                                                        // Normal.
        while (!isspace(*p) && *p != '\0') p++;
    }
    return p;
}
