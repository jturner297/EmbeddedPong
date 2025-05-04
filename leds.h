/**
**************************************************************************************************
* @file leds.h
* @brief Header file for program main
* @author Justin Turner
* @co-author Jesse Garcia
* @version Header for leds.c module
* ------------------------------------------------------------------------------------------------
* Declares function prototypes for leds.c
**************************************************************************************************
*/
#ifndef LEDS_H
#define LEDS_H

#include "main.h" //functions use external variables form main.h

void configure_LEDS (GPIO_TypeDef *port, uint32_t pins[], uint32_t number_of_pins, uint32_t port_clock_num);

void TURN_OFF_GAMEBOARD_LEDS (void);
void TURN_OFF_POINTS_DISPLAY (struct Player *p);
void TOGGLE_POINTS_DISPLAY (struct Player *p);
void UPDATE_POINTS_DISPLAY (struct Player *p);
void TURN_OFF_MISS_LEDS (struct Player *p, struct Player *opp );
void HANDLE_HITZONE_LEDS (struct Player *p, uint32_t curentTIME_ms);

#endif
