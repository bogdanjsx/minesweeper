##Minesweeper

==

An attempt at the famous minesweeper game, using the curses library

Compiler version:   gcc 4.7

External dependencies: curses library, get it executing the following command:
	`sudo apt-get install libncurses5-dev`

Recommended terminal size: 155x45

==

Game features:

- Multiple hitting

- Multiple flagging

- Color schemes

- Pause Game: Press the Escape key during a game to pause it and return to the main menu; this will pause the game time, but NOT the round time you can either continue the paused game or start a new one

- The two timers work this way:

	* Round time starts when "New Game" is selected and stops when this option is selected again, resetting. Does not stop during pause; 

	* Game time starts when the first cell is hit and stops when the game is over, be it successful or not. Stops during pause

- When a game is over, you can either press Escape to return to the main menu, or N to start a new game.

- Cells that are not hit yet are colored in white, while the other ones have the current color scheme color.

- If the game is lost, mines become yellow-ish and everything else turns white.
