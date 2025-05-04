/**
**************************************************************************************************
* @file input.h
* @brief Header file for program main
* @author Justin Turner
* @co-author Jesse Garcia
* @version Header for input.c module
* ------------------------------------------------------------------------------------------------
* Declares function prototypes for input.c
**************************************************************************************************
*/
#ifndef INPUT_H_
#define INPUT_H_
#include "main.h"

void configure_external_switches(void);
void configure_board_button (void);
void DEBOUNCE_PROTOCOL(struct UserInput *push_button, uint32_t currentTIME_ms);
void SPECIAL_BUTTON_ACTIONS(void);
void HANDLE_DEBOUNCED_BUTTON(void);
#endif /* INPUT_H_ */
