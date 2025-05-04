#include "game_logic.h"
#include "leds.h" //uses LED related functions from leds.c/h
#include "timers.h" //uses updateARR from timers.c/h
/**************************************************************************************************
* @file game_logic.c
* @brief  Source file for core game behavior and state transitions
* @author: Justin Turner
* @co-author: Jesse Garcia
* ------------------------------------------------------------------------------------------------
* Defines functions used for handling game logic and game related events
**************************************************************************************************/
//================================================================================================
// HANDLE_GAME()
// @parm: none
// @return: none
//         Called when the system state is in PLAY_MODE. Handles majority of the game's
//	   logic and systems
//================================================================================================
void HANDLE_GAME(void){
	switch(game_state){
	case INITIAL_SERVE:
		LEDcount = current_saved_position;//Places the ball at the saved position
		updateARR(pace = DEFAULT_SPEED);//reset speed
		startTIM2_MACRO;
		direction = RIGHT; //set direction right
		game_state = MOVE_RIGHT;
		break;
	case MOVE_RIGHT://ball is moving right
		if (LEDcount == RIGHT_HITZONE_POS ){ //if the ball has entered the right HITZONE
			game_state =RIGHT_HITZONE; }//change gamestate to RIGHT_HITZONE
		break;
	case RIGHT_HITZONE: //ball is in right HITZONE
		if (LEDcount == RIGHT_MISS_ZONE){ //the ball has left the HITZONE - right player "P2" missed
			HANDLE_MISS(&P2, &P1, msTimer);}
		break;
	case MOVE_LEFT://ball is moving left
		if (LEDcount == LEFT_HITZONE_POS){//if the ball has entered the left HITZONE
			game_state = LEFT_HITZONE; }//change gamestate to LEFT_HITZONE
		break;
	case LEFT_HITZONE://ball is in left HITZONE
		if (LEDcount == LEFT_MISS_ZONE){//the ball has left the HITZONE - left player "P1" missed
			HANDLE_MISS(&P1, &P2, msTimer);}
		break;
	case P1_LOST://The left player "P1" missed has lost the round
		TIME_OUT(&P1, msTimer); //enter timeout phase
		break;
	case P2_LOST://The right player "P2" missed has lost the round
		TIME_OUT(&P2, msTimer); //enter timeout phase
		break;
	case P1_WINNERS_CIRCLE://The left player "P1" has won the game
		IN_THE_WINNERS_CIRCLE(&P1, &P2, msTimer);
		break;
	case P2_WINNERS_CIRCLE://The right player "P2" has won the game
		IN_THE_WINNERS_CIRCLE(&P2, &P1, msTimer);
		break;
	}
}
//================================================================================================
// HANDLE_GAME_LED_MOVEMENT()
// @parm: nonr
// @return: none
//         Called within TIM2, Handles the automated led movement/animation.
//================================================================================================
void HANDLE_GAME_LED_MOVEMENT (void){
    switch(game_state){
    	case P1_WINNERS_CIRCLE:
    		TOGGLE_POINTS_DISPLAY(&P1);
    		break;
    	case P2_WINNERS_CIRCLE:
    		TOGGLE_POINTS_DISPLAY(&P2);
    		break;
    	default: //game is not in a winner's state
    		ToggleBoardLED_MACRO;
    		if (LEDcount > 1 && LEDcount < 22){ //if the led is in the GAMEZONE
     	    	   LEDS[LEDcount].port-> ODR &= ~(0x1 << LEDS[LEDcount].pin); //turn off current LED
     	    }
     	    switch(direction){ //increment or decrement the LEDcount based on the direction
     	        case LEFT:
     	        	LEDcount++;//increment
     	        	break;
     	        case RIGHT:
     	        	LEDcount--;//decrement
     	        	break;
     	     }
     	    if (LEDcount > 1 && LEDcount < 22){ //if the led is in the GAMEZONE
     	       LEDS[LEDcount].port-> ODR |= (0x1 << LEDS[LEDcount].pin); //turn on the new LED
     	    }
     	    break;
    }
}
//================================================================================================
// PRESS_DETECTED()
//
// @parm: *p - pointer to the Player struct
//        currentTIME_ms - current system time in milliseconds
// @return: none
//
//         When a press is detected, this function sets the pressed flag, stores the press timestamp,
//	   and turns off the player's hitzone LED to simulate a toggle behavior.
//================================================================================================
void PRESS_DETECTED(struct Player *p, uint32_t currentTIME_ms){
	p->pressedFLAG = 1;
	p->pressTIME_STAMP = currentTIME_ms; //takes note current time
	p->hitzoneLED.port-> ODR &= ~(0x1 <<   p->hitzoneLED.pin); //turn off hitzoneLED (simulate toggle behavior)
}
//================================================================================================
// UPDATE_SCORE()
//
// @parm: *p - pointer to the Player struct who lost the round
//        *opp - pointer to the opposing Player struct
// @return: none
//
//         Called when a player loses. Resets their score, updates the display, increments the
//		   opponent's score, and sets the winner flag if needed.
//================================================================================================
void UPDATE_SCORE (struct Player *p, struct Player *opp){
	p->score=0;//take away the players points
	TURN_OFF_POINTS_DISPLAY(p);//update player's points display
    opp->score++; //update opponents score
	if(opp->score == 3){ //the opponent has reach 3 points they have won the game
			opp->winnerFLAG=1;
	}
	UPDATE_POINTS_DISPLAY(opp); //update the opponent's points display
}
//================================================================================================
// TIME_OUT()
//
// @parm: p* - pointer to the Player struct who timed out
//        currentTIME_ms - current system time in milliseconds
// @return: none
//
//         Called when a player has lost a round. Temporarily turns off the GAMEBOARD LEDs and the
//	   built-in board LED for a declared TIME_OUT_TIME. After the time out time has passed, resets the
//	   player's miss flag, miss LED, and returns game state to INITIAL_SERVE.
//================================================================================================
void TIME_OUT (struct Player *p, uint32_t currentTIME_ms){
	TURN_OFF_GAMEBOARD_LEDS();
	TurnOffBoardLED_MACRO;
	stopTIM2_MACRO;
	if (currentTIME_ms - p->missTIME_STAMP >= TIME_OUT_TIME){//if the time out time has passed
		p->missFLAG = 0; //reset player miss flag
		p->missTIME_STAMP = 0;	//clear miss timestamp
		p->missLED.port-> ODR &= ~(0x1 <<   p->missLED.pin); //turn off miss LED
		game_state = INITIAL_SERVE;
	}
}
//================================================================================================
// SET_UP_WINNERS_CIRCLE()
//
// @parm: *p - pointer to the winning Player struct
//        currentTIME_ms - current system time in milliseconds
// @return: none
//
//         Prepares the system for the winner’s circle state by recording the timestamp of the win,
//	   turning off LEDs, and reconfiguring TIM2 to display the animation of the points display
//================================================================================================
void SET_UP_WINNERS_CIRCLE(struct Player *p, uint32_t currentTIME_ms){
	p->winnerTIME_STAMP = currentTIME_ms;//new
	TURN_OFF_GAMEBOARD_LEDS();
	stopTIM2_MACRO;
	configureTIM2();
	updateARR(8);
	startTIM2_MACRO;
}

//================================================================================================
// HANDLE_MISS()
//
// @parm: *p - pointer to the Player struct who missed
//        opp* - pointer to the opposing Player struct
//        currentTIME_ms - current system time in milliseconds
// @return: none
//
//         Called when a player misses. Updates miss timestamp and flag, turns on miss LED, calls
//	   UPDATE_SCORE(), and changes game state. If the opponent has won, enters the winner's circle.
//================================================================================================
void HANDLE_MISS(struct Player *p, struct Player *opp, uint32_t currentTIME_ms){
	p->hitzoneLED.port-> ODR &= ~(0x1 <<   p->hitzoneLED.pin); //force player HITZONE LED off
	p->missTIME_STAMP = currentTIME_ms; //create timestamp for player miss, will be used to turn off the miss LED
	p->missFLAG = 1;
	p->missLED.port-> ODR |= (0x1 <<   p->missLED.pin); //turn on player's miss LED
	UPDATE_SCORE(p, opp);//reset the players score and display, and update opponent's score and display
	current_saved_position = DEFAULT_POSITION; //reset the ball position to the default.
	if (opp->winnerFLAG == 1){ //if the opponets's winner flag was set in the UPDATE_SCORE function
		SET_UP_WINNERS_CIRCLE(opp, currentTIME_ms); //setup the conditions for the winner's circle
		if (opp->ID == TWO){//if the opponent is flagged as P2.....
			game_state = P2_WINNERS_CIRCLE;
			}
		else{ //they are P1
			game_state = P1_WINNERS_CIRCLE;
		}
	}
	else{//the opponent has not reached 3 points yet
		if (p->ID == TWO){//if the opponent is flagged as P2.....
			game_state = P2_LOST;
		}
		else{//they are P1
			game_state = P1_LOST;
		}
	}
}
//================================================================================================
// IN_THE_WINNERS_CIRCLE()
//
// @parm: *p - pointer to the winning Player struct
//        *opp - pointer to the opposing Player struct
//        currentTIME_ms - current system time in milliseconds
// @return: none
//
//         Waits 2.5 seconds after a player wins. Resets player score, LEDs, and sets game state
//	   to INITIAL_SERVE to restart the game.
//================================================================================================
void IN_THE_WINNERS_CIRCLE(struct Player *p, struct Player *opp, uint32_t currentTIME_ms){
	p->hitzoneLED.port-> ODR |= (0x1 <<   p->hitzoneLED.pin); //force green HITZONE LED on
	if(currentTIME_ms - p->winnerTIME_STAMP >= WINNERS_CIRCLE_TIME){//Winner's circle time is up
		stopTIM2_MACRO;
		p->score = 0;//reset score back to 0
		TURN_OFF_POINTS_DISPLAY(p); //turn off the winner's point's display
		opp->missFLAG = 0; //reset opponent miss flag
		opp->missLED.port-> ODR &= ~(0x1 <<   opp->missLED.pin); //turn off opponent miss LED
		p->winnerTIME_STAMP = 0; //clear player win time stamp
		p->winnerFLAG = 0;//clear player win flag stamp
		game_state = INITIAL_SERVE;
	}
}































