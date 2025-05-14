#include "timers.h"
/**
**************************************************************************************************
* @file timers.c
* @brief Source file for timer configuration and control functions
* @author: Justin Turner
* @corresponding author: Jesse Garcia
* ------------------------------------------------------------------------------------------------
* Defines functions used for timer control and configuration
**************************************************************************************************
*/
//================================================================================================
// configureSysTickInterrupt()
//
// @parm: none
// @return: none
//
// 		Configures the hardware so the SysTick timer will trigger every 1ms
//================================================================================================
void configureSysTickInterrupt(void)
{
	SysTick->CTRL = 0; //disable SysTick timer
	NVIC_SetPriority(SysTick_IRQn, 7); //set priority level at 7
	//SysTick->LOAD = FLASHfrequency[speeds]; //set the counter reload value
	SysTick->LOAD = 3999; //set the counter reload value 1ms
	SysTick->VAL = 0; //reset SysTick timer value
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; //use system clock
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; //enable SysTick interrupts
}

//================================================================================================
// configureTIM2()
//
// @parm: none
// @return: none
//
// 	Recalculates and sets TIM2's ARR register based on the specified speed. This directly
//	affects how often the timer overflows and triggers an interrupt. The timer counter (CNT)
//	is also reset to ensure immediate effect.
//
//       Note: The counter reset and ARR value is handled by the updateARR function
//	       The Timer does not start until the INITIAL_SERVE state.
//================================================================================================
void configureTIM2 (void)
{
		 RCC->APB1ENR1 |= (1 << 0);  // Enable TIM2 clock
	  	 TIM2->PSC = (SYS_CLK_FREQ/cntclk -1);
	  	 TIM2->DIER |= (1 << 0);          // Enable update interrupt
	     NVIC_SetPriority(TIM2_IRQn, 2); //set priority level at 2
	     NVIC_EnableIRQ(TIM2_IRQn);       // Enable interrupt in NVIC
}

//================================================================================================
// updateARR()
//
// @parm: speed = Desired update frequency (in HZ)
// @return: none
//
// 		 Recalculates and sets TIM2's ARR register based on the specified speed. This directly
//		 affects how often the timer overflows and triggers an interrupt. The timer counter (CNT)
//		 is also reset to ensure immediate effect.
//================================================================================================
void updateARR (uint32_t speed)
{
	  TIM2->ARR = (cntclk/speed - 1); //update ARR value
	  TIM2->CNT = 0; //reset timer counter
}
