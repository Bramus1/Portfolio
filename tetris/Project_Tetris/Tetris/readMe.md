### Tetris game 
This is a simple implementation of our Tetris. The game is built using object-oriented programming principles and consists of several classes, including Game, Board, Bag, Brick, Observable, and View.

### Features
- Random generation of tetromino shapes.
- Ability to move tetrominoes left, right, down, and rotate them.
- Scoring system based on the number of lines cleared simultaneously,current level and the height of the drop of tetromino.
- Pause and resume functionality.
- Restart the game.

### Gameplay

In order to switch between the Game played in console(Gui 1) and the Game played with an interface(Gui 0), go to the main.cpp  file and where I define Gui  swicht between 0 and 1.

In order to set up the game properly, the game starts by asking the user to enter :

- whether or not they want to resize  the board (minimum 20x10(standard board); maximum 30x25)
- whether or not they want to partially fill the board
- the winning score (minumum 100)
- the victory level (minimum 2; maximum 20)
- the number of cleared lines needed to win (minimum 10).
- the timer of the game.


- q : move the tetronimo left
- d : move the tetronimo right
- s : move the tetronimo down
- e : rotate the tetronimo clockwise
- x : to instantly drop the tetronimo
- u : level up ( I left this for testing purpose)
- a : rotate the tetronimo anticlockwise


The game ends when the stack of tetrominoes reaches the top of the playing field (GAME OVER), or if one of the winning parameters has been reached (VICTORY), or the time of the game ended.

### Bugs
So far, we did not find any bugs.

### Programmers

Giuliano Taurone 59333


