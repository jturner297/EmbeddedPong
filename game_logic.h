/**
**************************************************************************************************
* @file game_logic.h
* @brief Header file for program main
* @author Justin Turner
* @co-author Jesse Garcia
* @version Header for game_logic.c module
* ------------------------------------------------------------------------------------------------
* Declares function prototypes for game_logic.c
**************************************************************************************************
*/
#ifndef GAME_LOGIC_H_
#define GAME_LOGIC_H_

#include "main.h"

//function prototypes
void PRESS_DETECTED(struct Player *p, uint32_t currentTIME_ms);
void UPDATE_SCORE(struct Player *p, struct Player *opp);
void TIME_OUT(struct Player *p, uint32_t currentTIME_ms);
void SET_UP_WINNERS_CIRCLE(struct Player *p, uint32_t currentTIME_ms);
void HANDLE_MISS(struct Player *p, struct Player *opp, uint32_t currentTIME_ms);
void IN_THE_WINNERS_CIRCLE(struct Player *p, struct Player *opp, uint32_t currentTIME_ms);
void HANDLE_GAME(void);
void HANDLE_GAME_LED_MOVEMENT(void);
#endif /* GAME_LOGIC_H_ */
