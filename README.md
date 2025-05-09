# Embedded Pong
This project simulates a game of pong using thirty external LEDs and two push
button switches connected to the **NUCLEO-L476RG** development board. 

A fast-paced, real-time reaction game using external LEDs and physical buttons interfaced with NUCLEO-L476RG development board. 



## Overview
**Embedded Pong** is a two-player competitive game that simulates a physical game of Pong using **30 external LEDs** and two pushbuttons connected to the **STM32 NUCLEO-L476RG**. The LEDs are arranged linearly to represent a gameboard, with visual feedback for hits, misses, and scoring. Players compete by timing their button presses to return a "moving" LED that travels back and forth across the gameboard.

**Embedded Pong** is a two-player competitive game that simulates a game of pong using **30 external LEDs** and 

The HITZONE, MISS, and GAMEBOARD LEDs are arranged linearly: twenty-two GAMEBOARD LEDs in the center, plus one
green HITZONE LED and one red MISS LED at each end. Off to the side, each player has a set
of three white POINTS LEDs, to display their score. Multiple modes of operation are
provided.
  The first mode is called PLAY_MODE. In this mode, the GAMEBOARD LEDs simulate a "ball"
moving back and forth, while the two pushbuttons serve as paddles. Players attempt to hit
the ball when it enters their HITZONE. Each successful hit increases the ball's speed,
ramping up the difficulty in the process. When a player wins a round, one LED in their
points display lights up. The game is over when a player wins three rounds in a row, at
which point their points display will flash briefly to signify victory.
The second mode is called MOVE_MODE. In this mode, a single LED is manually shifted
left or right with button presses. This mode directly connects to PLAY_MODE by allowing
the player to position the starting LED for the first round.

## Features
- ⚙️ **Real-time LED animation** using TIM2 interrupt
- 🧠 **Debounced input handling** via EXTI interrupts
- 🔁 **Two modes of operation**:  
  - **PLAY_MODE** – the core game, LED ball movement + hit detection  
  - **MOVE_MODE** – manual LED control for choosing starting positions
- 💡 **30 external LEDs**:  
  - 22 blue GAMEBOARD LEDs  
  - 2 green HITZONE LEDs  
  - 2 red MISS LEDs  
  - 6 white POINTS LEDs (3 per player)
- 🎯 **Scoring system** with reset on miss and flashing win animation
- 🔄 **Mode toggling** using the on-board USER button (PC13)

## Hardware Requirements
- STM32 NUCLEO-L476RG development board
- 30 external LEDs:
  - 22 blue GAMEBOARD LEDs
  - 2 green HITZONE LEDs (1 per player)
  - 2 red MISS LEDs (1 per player)
  - 6 white POINTS LEDs (3 per player)
- 2 pushbuttons (connected to PA4 and PA1)
- Breadboard and jumper wires
- USB cable for power and flashing via ST-Link

NOTE:
  -Changing modes in achieved by pressing the "SPECIAL" built in button on the NUCLEO board.
	-The built-in LED is solid in MOVE_MODE and blinks in PLAY_MODE.
