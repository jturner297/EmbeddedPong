#include <stdint.h>
#include <stm32l476xx.h>
#include "main.h"
#include "leds.h"
#include "game_logic.h"
#include "timers.h"
#include "input.h"
/**
**************************************************************************************************
* @file main.c
* @brief Main program body
* @author: Justin Turner
* @co-author: Jesse Garcia
* @version 2.0
* ------------------------------------------------------------------------------------------------
*  Pong Game Project
*  			This project simulates a game of pong using thirty external LEDs and two push
*  		button switches connected to the NUCLEO-L476RG development board. The HITZONE, MISS, and
*  		GAMEBOARD LEDs are arranged linearly: twenty-two GAMEBOARD LEDs in the center, plus one
*  		green HITZONE LED and one red MISS LED at each end. Off to the side, each player has a set
*  		of three white POINTS LEDs, to display their score. Multiple modes of operation are
*  		provided.
*  			The first mode is called PLAY_MODE. In this mode, the GAMEBOARD LEDs simulate a "ball"
*  		moving back and forth, while the two pushbuttons serve as paddles. Players attempt to hit
*  		the ball when it enters their HITZONE. Each successful hit increases the ball's speed,
*  		ramping up the difficulty in the process. When a player wins a round, one LED in their
*  		points display lights up. The game is over when a player wins three rounds in a row, at
*  		which point their points display will flash briefly to signify victory.
*			The second mode is called MOVE_MODE. In this mode, a single LED is manually shifted
*		left or right with button presses. This mode directly connects to PLAY_MODE by allowing
*		the player to position the starting LED for the first round.
*	NOTE:
*	-Changing modes in achieved by pressing the "SPECIAL" built in button on the NUCLEO board.
*	-The built-in LED is solid in MOVE_MODE and blinks in PLAY_MODE.
**************************************************************************************************
*/
volatile uint32_t LEDcount;//Sets the first LED that will be turned on
enum directions direction;
enum game_states game_state = INITIAL_SERVE;
enum system_states system_state = PLAY_MODE;
volatile uint32_t pace = DEFAULT_SPEED;

struct UserInput button = {0, 0, 0}; //Creates a button with choice, counter, pending values at 0;
volatile uint32_t current_saved_position = DEFAULT_POSITION; //sets default "ball" position
volatile uint32_t msTimer = 0;

struct Light_Emitting_Diode LEDS[] = { //declaring the array of LEDS
		 {GPIOC, 8}, {GPIOC, 9}, {GPIOC, 6}, {GPIOB, 8}, {GPIOC, 5}, {GPIOB, 9},
		 {GPIOA, 12}, {GPIOA, 11}, {GPIOA, 6}, {GPIOB, 12}, {GPIOA, 7}, {GPIOB, 11},
		 {GPIOB, 6}, {GPIOC, 7}, {GPIOB, 2}, {GPIOA, 9}, {GPIOB, 1}, {GPIOA, 8},
		 {GPIOB, 15}, {GPIOB, 10}, {GPIOB, 14}, {GPIOB, 4}, {GPIOB, 13}, {GPIOB, 5}
};

struct Player P1 = {//left player
		.score =0,

		.pressTIME_STAMP = 0,
		.missTIME_STAMP = 0,
		.winnerTIME_STAMP = 0,

		.missFLAG = 0,
		.pressedFLAG = 0,
		.winnerFLAG =0,

		.points_display = { {GPIOC, 0}, {GPIOC, 3},{GPIOC, 2} },
		.hitzoneLED = {GPIOB, 13},
		.missLED = {GPIOB, 5}
};

struct Player P2 = { //right player
		.ID = TWO,
		.score =0,

		.pressTIME_STAMP = 0,
		.missTIME_STAMP = 0,
		.winnerTIME_STAMP =0,

		.missFLAG = 0,
		.pressedFLAG = 0,
		.winnerFLAG =0,

		.points_display = { {GPIOC, 12}, {GPIOC, 10}, {GPIOC, 11} },
		.hitzoneLED = {GPIOC, 9},
		.missLED = {GPIOC, 8}
};





int main(void)
{
	//--initialize hardware------------------------------------------------------------------------
	//LEDs connected to port A's pins
	uint32_t GPIOA_pins[] = {6, 7, 8, 9, 11, 12,5};

	//LEDs connected to port B's pins
	uint32_t GPIOB_pins[] = {1, 2, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15};

	//LEDs connected to port C's pins
	uint32_t GPIOC_pins[] = {5, 6, 7, 8, 9 ,10,11,12, 0,3,2};

	configure_LEDS(GPIOA, GPIOA_pins, GPIOApins_used, 0);//configure port A's LEDs
	configure_LEDS(GPIOB, GPIOB_pins, GPIOBpins_used, 1);//configure port B's LEDs
	configure_LEDS(GPIOC, GPIOC_pins, GPIOCpins_used, 2);//configure port C's LEDs
	configure_external_switches();//configure the switches and their dedicated interrupts
	configure_board_button(); //configure the board button and its interrupt
	configureSysTickInterrupt();
	startSysTickTimer_MACRO;

	configureTIM2(); //configure general purpose TIM2


	//---------------------------------------------------------------------------------------------
	// This main loop continuously monitors both system and game states. It responds to player
	// input and updates gameplay accordingly, managing LED movement, scoring, and state changes
	//---------------------------------------------------------------------------------------------
	while (1)
	{
		switch(system_state){
		case PLAY_MODE:
			HANDLE_GAME(); //see game_logic.c/h
			break;
		case MOVE_MODE:
			TurnOnBoardLED_MACRO;//Turn on the board LED
			LEDS[LEDcount].port-> ODR |= (0x1 << LEDS[LEDcount].pin); //turn on current LED
			current_saved_position = LEDcount;//saves the current LED position for when the user switches modes
			break;
		}//end switch
		if(button.press_pending == 1 && msTimer - button.debounce_counter >= DEBOUNCE_DELAY  ){//if debouncing is finished...
			HANDLE_DEBOUNCED_BUTTON(); //see input.c/h
			button.press_pending = 0; // clear the pending flag
			button.debounce_counter = 0;//clear debounce counter;
		}
	}//end while loop
}//end main


//================================================================================================
// EXTI4_IRQHandler()
//
// @parm: none
// @return: none
//
// 		 When triggered, it identifies the button as the LEFT button,
//       stores the ID, and begins the debounce protocol.
//================================================================================================
void EXTI4_IRQHandler(void)
{
	if (EXTI->PR1 & (0x1 << 4)) {//if the interrupt flag is set....
		EXTI->PR1 |= (0x1 << 4);  // Clear interrupt flag
		button.choice = Left_Pushed;//set button ID as "ONE"
		DEBOUNCE_PROTOCOL(&button, msTimer);//Initiate debouncing
	}
}
//================================================================================================
// EXTI1_IRQHandler()
//
// @parm: none
// @return: none
//
// 		 When triggered, it identifies the button as the RIGHT button,
//       stores the ID, and begins the debounce protocol.
//================================================================================================
void EXTI1_IRQHandler(void)
{
	if (EXTI->PR1 & (0x1 << 1)) { //if the interrupt flag is set....
		  EXTI->PR1 |= (0x1 << 1);  // Clear interrupt flag
		  button.choice = Right_Pushed;//set button ID as "TWO"
		  DEBOUNCE_PROTOCOL(&button, msTimer); //Initiate debouncing
	}
}








//================================================================================================
// EXTI15_10_IRQHandler()
//
// @parm: none
// @return: none
//
// 		 When triggered, it identifies the button as the SPECIAL button,
//       stores the ID, and begins the debounce protocol.
//================================================================================================
void EXTI15_10_IRQHandler(void)
{
	if (EXTI->PR1 & (0x1 << 13)) { //if the interrupt flag is set....
		EXTI->PR1 |= (0x1 << 13);  // Clear interrupt flag
		button.choice = Special_Pushed;//set button ID as "SPECIAL"
	    DEBOUNCE_PROTOCOL(&button, msTimer);//Initiate debouncing
	}
}
//================================================================================================
// SysTick_Handler()
//
// @parm: none
// @return: none
//
// 		  Increments the global millisecond timer (msTimer). If the system is in PLAY_MODE, it
//		  updates the hitzone LEDs for both players based on their input timing.
//================================================================================================
void SysTick_Handler(void)
{
	msTimer++; //goes up every 1ms
	if (system_state == PLAY_MODE){
		HANDLE_HITZONE_LEDS(&P1, msTimer);
		HANDLE_HITZONE_LEDS(&P2, msTimer);
	}
}
//================================================================================================
// TIM2_IRQHandler()
//
// @parm: none
// @return: none
//
// 		 If the game is in a winner's state, toggles the winner's point display. Otherwise, it is
// 		 responsible for updating the LED position.
//================================================================================================
void TIM2_IRQHandler(void)
{
	if (TIM2->SR & (1 << 0)) {
        TIM2->SR &= ~(1 << 0);// Clear update flag
        HANDLE_GAME_LED_MOVEMENT(); //see game_logic.c/h
	}
}













































