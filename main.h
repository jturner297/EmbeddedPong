/**
**************************************************************************************************
* @file main.h
* @brief Header file for program main
* @author Justin Turner
* @co-author Jesse Garcia
* @version Header for main.c module
* ------------------------------------------------------------------------------------------------
* Defines the shared constants, macros, enums, structures, and declarations used in the program
**************************************************************************************************
*/
#ifndef MAIN_H
#define MAIN_H
#include <stdint.h>
#include "stm32l476xx.h"

//macros
#define startSysTickTimer_MACRO (SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk)
#define startTIM2_MACRO (TIM2->CR1 |= (1 << 0)) //start timer
#define stopTIM2_MACRO (TIM2->CR1 &= ~(1 << 0)) //stop timer
#define ToggleBoardLED_MACRO (GPIOA->ODR ^= (0x1 << 5))
#define TurnOnBoardLED_MACRO (GPIOA->ODR |= (0x1 << 5))
#define TurnOffBoardLED_MACRO (GPIOA->ODR &= ~(0x1 << 5))
#define RightButtonPressed ((!(GPIOA->IDR & (0x1 << 1))))//macro
#define LeftButtonPressed ((!(GPIOA->IDR & (0x1 << 4)))) //macro
#define SpecialButtonPressed ((!(GPIOC->IDR & (0x1 << 13))))//macro
#define NUM_of_LEDS 30
#define GPIOApins_used 7 //number of GPIOA pins used for LEDs
#define GPIOBpins_used 13 //number of GPIOB pins used for LEDs
#define GPIOCpins_used 11 //number of GPIOC pins used for LEDs
#define SYS_CLK_FREQ 4000000// default frequency of the device = 4 MHZ
#define cntclk 1000
#define DEBOUNCE_DELAY 20//20ms debounce delay
#define TIME_OUT_TIME 1800//arbitrary value that felt the best (MS)
#define HITZONE_LED_TOGGLE_TIME 150//arbitrary value that felt the best (MS)
#define WINNERS_CIRCLE_TIME 2500//arbitrary value that felt the best (MS)
#define DEFAULT_SPEED 5 //initial speed for LED movement, it felt the best
#define DEFAULT_POSITION 12 //initial position of the "ball"
#define RIGHT_MISS_ZONE 0
#define LEFT_MISS_ZONE 23
#define RIGHT_HITZONE_POS 1
#define LEFT_HITZONE_POS 22

//enums
enum choices { Left_Pushed = 1, Right_Pushed, Special_Pushed };
enum game_states { INITIAL_SERVE, MOVE_RIGHT, RIGHT_HITZONE, MOVE_LEFT,
	LEFT_HITZONE, P1_LOST, P2_LOST, P1_WINNERS_CIRCLE, P2_WINNERS_CIRCLE };
enum directions {LEFT, RIGHT};
enum identifications{ONE, TWO};
enum system_states {PLAY_MODE, MOVE_MODE};

//Structures
struct UserInput{
	volatile uint32_t choice; //Determines which button was pressed
	volatile uint32_t debounce_counter; //Counter for debouncing
	volatile uint32_t press_pending; //Flag for pending button press
};

struct Light_Emitting_Diode{
	GPIO_TypeDef *port; //GPIOx
	uint32_t pin; //Pin number
};

struct Player{
	enum identifications ID; //The player has a unique identifier, either one or two
	uint32_t score; //Controls the player's score
	volatile uint32_t pressTIME_STAMP;//Time stamp created when the player presses their button
	volatile uint32_t missTIME_STAMP;//Time stamp created when the player loses a round
    uint32_t winnerTIME_STAMP;//Time stamp created when the player wins the game
	volatile uint32_t missFLAG;//Flag that is set when the player loses a round
	volatile uint32_t pressedFLAG;//Flag that is set when the player presses their button
    uint32_t winnerFLAG;//Flag that is set when the player wins the game
	struct  Light_Emitting_Diode points_display[3];//LEDs that denote a player's score
	struct  Light_Emitting_Diode hitzoneLED;//Dedicated hitzone LED that is always lit. Toggles when the player presses their button
	struct  Light_Emitting_Diode missLED;//Dedicated miss LED, that lights up when the player loses
};

extern enum system_states system_state;
extern enum game_states game_state;
extern volatile uint32_t current_saved_position;
extern volatile uint32_t msTimer;
extern volatile uint32_t pace;
extern volatile uint32_t LEDcount;
extern  enum directions direction;
extern struct UserInput button;
extern struct Player P1, P2;
extern struct Light_Emitting_Diode LEDS[];

#endif /* MAIN_H */
