/**
**************************************************************************************************
* @file timers.h
* @brief Header file for program main
* @author Justin Turner
* @corresponding author: Jesse Garcia
* @version Header for timers.c module
* ------------------------------------------------------------------------------------------------
* Declares function prototypes for timers.c
**************************************************************************************************
*/
#ifndef TIMERS_H_
#define TIMERS_H_

#include "main.h"

void configureSysTickInterrupt(void);
void configureTIM2 (void);
void updateARR (uint32_t speed);

#endif /* TIMERS_H_ */
