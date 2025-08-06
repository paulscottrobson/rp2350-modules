// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      queue.c
//      Purpose :   Input Queue for keyboard
//      Date :      6th August 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "input_module.h"
#include "input_module_local.h"

#define INPQUEUESIZE    (128)                                                       // So we can unpack function keys !

uint8_t  queueEntryCount = 0;                                                       // The input queue
uint16_t keyboardQueue[INPQUEUESIZE];
char     *functionKeyText[12];                                                      // Function key text, if any.

/**
 * @brief      Empty the keyboard queue, reset the function keys.
 */
void INPResetQueue(void) {
    queueEntryCount = 0;                                                            // Empty the queue.
    for (int i = 0;i < 12;i++) functionKeyText[i] = NULL;                           // Erase the function key definitions.
}

/**
 * @brief      Set the text associated with a function key.
 *
 * @param[in]  keyNumber  Function key number (1-12)
 * @param      text       Text to be associated with it, or NULL to disable.
 */
void INPSetFunctionKey(uint8_t keyNumber,char *text) {
    if (keyNumber >= 1 && keyNumber <= 12) {
        functionKeyText[keyNumber-1] = text;
    }
}
/**
 * @brief      Insert a charactr into the keyboard queue
 *
 * @param[in]  key   Code of character to insert
 */
void INPInsertIntoQueue(int16_t key) {
    if (queueEntryCount == INPQUEUESIZE) return;                                    // Queue is full.

    if (key >= CTL_F1 && key <= CTL_F12) {                                          // Function key to be inserted.
        char *text = functionKeyText[key-CTL_F1];                                   // What if anything is assigned to it ?
        if (text != NULL) {                                                         // If something, push it into the queue.
            while (*text != '\0') INPInsertIntoQueue(*text++);                      // Queue limits stop infinite recursion.
            return;
        }
    }
    keyboardQueue[queueEntryCount++] = key;
    // LOG("Added %d to queue",key);
}

/**
 * @brief      Get the next key in the queue.
 *
 * @return     Key at the front of the queue, zero if there is none.
 */
uint32_t INPGetKey(void) {
    if (queueEntryCount == 0) return 0;                                             // Queue empty, so return zero.
    int key = keyboardQueue[0];                                                     // Preserve the head of the queue.
    for (int i = 0;i < queueEntryCount-1;i++) {                                     // Remove head of the queue
        keyboardQueue[i] = keyboardQueue[i+1];
    }
    queueEntryCount--;                                                              // Fix up the count.
    return key;
}

/**
 * @brief      Check if a key is available in the queue
 *
 * @return     true if there is a key.
 */
bool INPIsKeyAvailable(void) {
    return queueEntryCount != 0;
}

