# Embedded Pong

<img src="assets/Embedded_PONG_thumbnailGIF.gif" width="500" />

A fast-paced, real-time reaction game using external LEDs and physical buttons interfaced with the **NUCLEO-L476RG development board**. 

## Overview
**Embedded Pong** is a two-player competitive game that simulates a game of Pong using **30 external LEDs** and **2 external pushbuttons** connected to the **NUCLEO-L476RG development board**. The LEDs are arranged linearly to represent the gameboard/table, with visual feedback for hits, misses, and scoring. The goal is to time button presses as the "moving" LED enters the player's HITZONE.

## Demos
<table>
  <tr>
    <td>
  <a href="https://www.youtube.com/watch?v=BQ9yHDXTO0I">
    <img src="assets/extended demo thumbnail.jpg" width="300%" />
      </a>
    </td>
    <td>
    <a href="https://www.youtube.com/watch?v=gO6LgwVbm_k">
    <img src="assets/move_mode demo thumbnail.jpg"" width="300%" />
      </a>
    </td>
  </tr>
  <tr>
    <td><sub><strong>Gameplay</strong></sub></td>
    <td><sub><strong>Mode Switching</strong></sub></td>
  </tr>
</table>


## Features
🎮 **Two-player competitive gameplay**
  - Players use physical buttons to hit a moving LED "ball" back and forth
  - Real-time hit detection when the ball enters the HITZONE

🔄 **Dual gameplay modes**
  - **PLAY_MODE** – The core game: LED "ball" movement, hit detection, scoring
  - **MOVE_MODE** – Manual LED control for choosing initial serve position

💡 **Full LED-based visual interface**
  - 22 blue GAMEBOARD LEDs 
  	- simulate ball movement
  - 2 green HITZONE LEDs (1 per player)
  	-  indicate valid hit regions
  - 2 red MISS LEDs (1 per player)
  	- light up when a player misses or presses too early
  - 6 white POINTS LEDs (3 per player)
  	- track player score
   	- animate when a player wins

⚙️ **State machine-driven architecture**
  - Clearly defined game phases: serve, movement, hitzone, miss, win animation
  - Transitions managed through timers and input state

⚡ **Hit detection and reaction feedback**
  - HITZONE LEDs toggle off briefly when a button is pressed
  - Timed LED feedback provides clear input acknowledgment

🕒 **Real-time processing using timers**
  - TIM2 used to control LED movement speed and animation
  - SysTick used for millisecond timekeeping and debouncing

👆 **Interrupt-based input handling**
  - External interrupts on PA1, PA4, and PC13
  - Software debouncing ensures clean button logic

🧠 **Scoring and win logic**
  - Score resets on a miss
  - First player to win 3 consecutive rounds wins the game
  - Flashing POINTS LEDs indicate game win/victory

🔁 **Instant Mode switching**
  - Pressing the built-in USER button toggles between MOVE_MODE and PLAY_MODE

🧪 **Low-level embedded C**
  - Written without HAL; uses direct register manipulation (CMSIS)


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

## Block Diagram
<img src="assets/EMBEDDED_PONG- BLOCK DIAGRAM.png" alt="Game Thumbnail" width="1000"/> 
