#include "input.h"
#include "timers.h" //uses updateARR from timers.c/h
#include "game_logic.h" //game related functions from game_logic.c/h
#include "leds.h" //uses LED related functions from leds.c/h
/**************************************************************************************************
* @file input.c
* @brief Source file for button configuration, debouncing, and input handling logic
* @author: Justin Turner
* @corresponding author: Jesse Garcia
* ------------------------------------------------------------------------------------------------
* Defines the functions responsible for input configuration and their associated interrupts. Also
* implements functions used for handling button presses.
*************************************************************************************************/
//================================================================================================
// configure_external_switches()
// @parm: none
// @return: none
// 	Setups 2 external push button switches at PA4 and PA1 for input. Also configures external
//	interrupts for pins 4 and 1
//  	Note: Left Button = PA4, Right Button = PA1
//================================================================================================
void configure_external_switches(void){
		RCC->AHB2ENR |= (0x1 << 0); //Port A - already enabled in previous function
		GPIOA->MODER &= ~(0x3 << (2 *4 ));//input pin (PA4 = 00)
		GPIOA->PUPDR = (GPIOA->PUPDR &= ~(0x3 << (2*4)))|(0x1 << 2 * 4);//toggles pull-up (PA4=01)
		//configure switch #2 PA1 for input
		GPIOA->MODER &= ~(0x3 << (2 * 1));//input pin (PA1 = 00)
		GPIOA->PUPDR = (GPIOA->PUPDR &= ~(0x3 << (2*1)))|(0x1 << 2 * 1);//toggles pull-up (PA1=01)

	    RCC->APB2ENR |= (0x1 << 0); //Enable system configuration clock for external interrupts
		SYSCFG->EXTICR[0] |= (0x0 << 4);//setup PA1 as target pin for EXTI1
		SYSCFG->EXTICR[1] |= (0x0 << 0);//setup PA4 as target pin for EXTI4
     	EXTI -> FTSR1 |= (0x1 << 4)| (0x1 << 1); //enable falling edge trigger detection
     	EXTI -> IMR1 |= (0x1 << 4)|(0x1 << 1); //Unmask EXTI4 and EXTI1
    	NVIC_SetPriority(EXTI4_IRQn, 6); //pin 4 interrupt: priority level 6
    	NVIC_SetPriority(EXTI1_IRQn, 5);//pin 1 interrupt: priority level 5
    	NVIC_EnableIRQ(EXTI4_IRQn); //enable interrupt at pin 4
    	NVIC_EnableIRQ(EXTI1_IRQn); //enable interrupt at pin 1
}
//================================================================================================
// configure_board_button()
// @parm: none
// @return: none
// 	Setup user button on Nucleo-L476RG board (PC13) for input. Also configures external
//	interrupts at PC13
//================================================================================================
void configure_board_button (void){
	//RCC->AHB2ENR |= (0x1 << 2); //Port C -already enabled in previous function
	GPIOC->MODER &= ~(0x3 << (2*13));
	GPIOC->PUPDR &= ~(0x3 << (2*13)); //Already uses hardware pull-up
	//RCC->APB2ENR |= (0x1 << 0); //Already done in previous function
	SYSCFG->EXTICR[3] |= (0x2 << 4); //setup PC13 as target pin for EXTI15_10
	EXTI -> FTSR1 |= (0x1 << 13);//enable falling edge trigger detection
	EXTI->IMR1 |= (0x1 << 13);//Unmask EXTI15_10
	NVIC_SetPriority(EXTI15_10_IRQn, 3);//pin 13 interrupt: priority level 2
	NVIC_EnableIRQ(EXTI15_10_IRQn); //enable interrupt at pin 13
}
//================================================================================================
// DEBOUNCE_PROTOCOL()
// @parm: push_button = Pointer to the UserInput struct representing the button
// 	  currentTIME_ms = Current system time in milliseconds.
// @return: none
//	 Flags the button press as pending and records the current time to track
//       how long the button has been held.
//================================================================================================
void DEBOUNCE_PROTOCOL(struct UserInput *push_button, uint32_t currentTIME_ms){
	push_button->press_pending = 1; //flag that determines if a press is pending
	push_button->debounce_counter = currentTIME_ms;//creates a timestamp for the debounce
}
//================================================================================================
// SPECIAL_BUTTON_ACTIONS()
// @parm: none
// @return: none
//	Used to switch game modes and cleanly reinitialize the game state.
//================================================================================================
void SPECIAL_BUTTON_ACTIONS(void){
	LEDcount = (current_saved_position); //saves the LEDcount position from the MOVE_MODE - Will be used in the first round for the PLAY MODE
	stopTIM2_MACRO; //stop the timer
	TURN_OFF_POINTS_DISPLAY(&P1);
	TURN_OFF_POINTS_DISPLAY(&P2);
	TURN_OFF_GAMEBOARD_LEDS();
	TURN_OFF_MISS_LEDS(&P1, &P2);
	P1.hitzoneLED.port-> ODR |= (0x1 <<   P1.hitzoneLED.pin); //force HITZONE LED on
	P2.hitzoneLED.port-> ODR |= (0x1 <<   P2.hitzoneLED.pin);//force HITZONE LED on
	P1.score = 0; //reset P1 score
	P2.score = 0;//reset P2 score
	game_state = INITIAL_SERVE;
	system_state^=1; //toggle system state
}

//================================================================================================
// HANDLE_DEBOUNCED_BUTTON()
//
// @parm: none
// @return: none
//
//	Called when the button is finished deboucning. It determines which button was pressed
//	(left, right, or special) and executes the appropriate behavior based on the current
//	system state.
//	- In PLAY_MODE:
//              Processes hit detection and scoring.
//              Handles valid hits and missed hits
//	- In MOVE_MODE:
//              Allows the user to manually shift the LED for initial position setup.
//	- Special Button:
//              Toggles game modes and handles resets.
//================================================================================================
void HANDLE_DEBOUNCED_BUTTON(void){
	switch(button.choice){
	case Left_Pushed: //left button pressed
		if (LeftButtonPressed){ //(DOUBLE CHECKING)if the left button is still pressed...
			switch(system_state){
			case PLAY_MODE: //if PLAY_MODE....
				//--if the game is not in a winner's state.........
				if(game_state!=P1_WINNERS_CIRCLE && game_state!=P2_WINNERS_CIRCLE && game_state!=P1_LOST && game_state!=P2_LOST){
					PRESS_DETECTED(&P1, msTimer); //initial press actions
				}
				if (game_state == LEFT_HITZONE && LEDcount ==LEFT_HITZONE_POS){ //if valid hit detected....
					updateARR(pace++); //increase speed
					game_state = MOVE_RIGHT;
					direction = RIGHT;
				}
				else if (game_state == MOVE_LEFT){ // if pressed while the ball was still moving towards HITZONE
					HANDLE_MISS(&P1, &P2, msTimer);
				}
				break;
			case MOVE_MODE: //if MOVE_MODE
				LEDcount++; //move left/increment
				if (LEDcount == 22){//if the LEDcount is 22
					LEDcount = 2;//change it back to 2
				}
				//--calculate previous LED------------------------------------------------------------------------
				//if LEDcount > 2, prevLED = LEDcount - 1, else the LEDcount is 2 the prevLED is 21
				uint32_t prevLED = (LEDcount > 2)? LEDcount - 1: 21;
				LEDS[prevLED].port-> ODR &= ~(0x1 << LEDS[prevLED].pin); //turn off the previous LED
				break;
			}
		}
		break;
	case Right_Pushed: //right button pressed
		if (RightButtonPressed){ //(DOUBLE CHECKING)if the left button is still pressed...
			switch(system_state){
			case PLAY_MODE:
				if(game_state!=P1_WINNERS_CIRCLE && game_state!=P2_WINNERS_CIRCLE && game_state!=P1_LOST && game_state!=P2_LOST){
					PRESS_DETECTED(&P2, msTimer); //initial press actions
				}
				if (game_state == RIGHT_HITZONE && LEDcount == RIGHT_HITZONE_POS){
					updateARR(pace++);
					game_state = MOVE_LEFT;
					direction = LEFT;
				}
				else if (game_state == MOVE_RIGHT){ //you hit too early lose
					HANDLE_MISS(&P2, &P1, msTimer);
				}
				break;
			case MOVE_MODE:
				LEDcount--;//move right/decrement
				if (LEDcount == 1){//if the LEDcount is 1.....
					LEDcount = 21;//change it back to 21
				}
				//--calculate previous LED------------------------------------------------------------------------
				//if LEDcount < 21, prevLED = LEDcount + 1, else the LEDcount is 21 the prevLED is 2
				uint32_t prevLED = (LEDcount < 21)? LEDcount + 1: 2;
				LEDS[prevLED].port-> ODR &= ~(0x1 << LEDS[prevLED].pin); //turn off the previous LED
				break;
			}//end switch
		}
		break;
	case Special_Pushed://board button pressed
		if (SpecialButtonPressed){
			SPECIAL_BUTTON_ACTIONS();
		}
		break;
	}
}

