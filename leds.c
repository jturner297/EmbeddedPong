#include "leds.h"
/**
**************************************************************************************************
* @file leds.c
* @brief Source file for LED configuration and control functions
* @author: Justin Turner
* @corresponding author: Jesse Garcia
* ------------------------------------------------------------------------------------------------
* Defines functions used for LED control and configuration.
*************************************************************************************************/
//================================================================================================
// configure_LEDS()
// @parm:   *port = GPIOx
//			pins[] = Array containing the port pin numbers used
//          number_of_pins = Number of port pins used
//			port_clock_num = The number associated with the port clock "0 for port A"
// @return: none
// 		Conveniently configures multiple LEDs at once. Also assists with board wire management.
//================================================================================================
void configure_LEDS (GPIO_TypeDef *port, uint32_t pins[], uint32_t number_of_pins, uint32_t port_clock_num)
{
	RCC->AHB2ENR |= (0x1 << port_clock_num);
	for (uint32_t i = 0; i < number_of_pins; i++){
		port->MODER &= ~(0x3 << (2 * pins[i]));
		port->MODER |= (0x1 << (2 * pins[i]));
		port->OTYPER &= ~(0x1 << pins[i]);
		port->OSPEEDR &= ~(0x3 << (2 * pins[i]));
		port->PUPDR &= ~(0x3 << (2 * pins[i]));
	}
}
//================================================================================================
// TURN_OFF_GAMEBOARD_LEDS()
// @parm: none
// @return: none
// 		Turns off GAMEBOARD LEDS (EVERY LED except the two HITZONE and two MISS LEDs)
//================================================================================================
void TURN_OFF_GAMEBOARD_LEDS (void)
{
	for (uint32_t i = 2; i<22;i++){
		LEDS[i].port-> ODR &= ~(0x1 << LEDS[i].pin);
	}
}
//================================================================================================
// TURN_OFF_POINTS_DISPLAY()
// @parm: *p = pointer to the Player struct
// @return: none
// 		Turns off the Players points-display
//================================================================================================
void TURN_OFF_POINTS_DISPLAY (struct Player *p)
{
	for (uint32_t i = 0; i<3;i++){
		p->points_display[i].port-> ODR &= ~(0x1 << p->points_display[i].pin);
	}
}
//================================================================================================
// UPDATE_POINTS_DISPLAY()
// @parm: *p = pointer to the Player struct
// @return: none
// 		Turns on the LEDs corresponding to the player's current score
//================================================================================================
void UPDATE_POINTS_DISPLAY (struct Player *p)
{
	for (uint32_t i = 0; i<p->score;i++){
		p->points_display[i].port-> ODR |= (0x1 << p->points_display[i].pin);
	}
}
//================================================================================================
// TOGGLE_POINTS_DISPLAY()
// @param:  *p = pointer to the Player struct
// @return: None
// 		Toggles all LEDs in the player's score display. Called within TIM2 to simulate blink effect
//================================================================================================
void TOGGLE_POINTS_DISPLAY (struct Player *p)
{
	for (uint32_t i = 0; i<3;i++){
		p->points_display[i].port-> ODR ^= (0x1 << p->points_display[i].pin);
	}
}
//================================================================================================
// TURN_OFF_MISS_LEDS()
// @param:  *p = pointer to the Player struct
//			*opp - pointer to the opposing Player struct
// @return: None
// 		Turns off the miss LEDs for both players.
//================================================================================================
void TURN_OFF_MISS_LEDS (struct Player *p, struct Player *opp )
{
		p->missLED.port-> ODR &= ~(0x1 << p->missLED.pin);
		opp->missLED.port-> ODR &= ~(0x1 << opp->missLED.pin);
}
//================================================================================================
// HANDLE_HITZONE_LEDS()
// @param:  *p = pointer to the Player struct
//			curentTIME_ms = current time in MS
// @return: None
// 		Called within the SysTick Handler. Ensures the HITZONE LEDs are always on and that they will
// 		eventually come back on after being turned off.
//================================================================================================
void HANDLE_HITZONE_LEDS (struct Player *p, uint32_t curentTIME_ms){
	if(p->pressedFLAG == 0 && p->missFLAG == 0 && game_state != 7 && game_state !=8){ //if p2 button is not pressed and the player has not missed
		p->hitzoneLED.port-> ODR |= (0x1 <<   p->hitzoneLED.pin); //turn on the p hitzone
		}
	else if ((curentTIME_ms - p->pressTIME_STAMP) >= HITZONE_LED_TOGGLE_TIME ){//if the toggle time has passed
		p->pressTIME_STAMP = 0; //clear time stamp
		p->pressedFLAG = 0; //clear press flag
	}
}
