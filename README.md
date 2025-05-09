Pong Game Project
  This project simulates a game of pong using thirty external LEDs and two push
button switches connected to the NUCLEO-L476RG development board. The HITZONE, MISS, and
GAMEBOARD LEDs are arranged linearly: twenty-two GAMEBOARD LEDs in the center, plus one
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

NOTE:
  -Changing modes in achieved by pressing the "SPECIAL" built in button on the NUCLEO board.
	-The built-in LED is solid in MOVE_MODE and blinks in PLAY_MODE.
