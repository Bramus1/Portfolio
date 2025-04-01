### Othello Game
This project implements a simple Othello game (also known as Reversi) using JavaFX. Othello is a two-player strategy board game where the goal is to have the majority of disks turned to display your color when the last playable empty square is filled.

### How to play
- Upon starting the game, you'll be prompted to enter the size of the game board and choose the game mode.
- If you select "Player vs Strategy" mode, you'll need to choose a strategy for the computer player.
- Once the game starts, click on the cells of the game board to place your pieces.
- The game will alternate between players, and the pieces will be flipped according to the rules of     Othello.
- The game ends when no more moves are possible for both players.
- The winner is determined by the player with the most pieces on the board.
- If you are playing vs a strategy and you dont have moves available, click on the Board to let the  strategy play its turn.

### Features
- Player vs Player mode: Play against another human player on the same computer.
- Player vs Strategy mode: Play against the computer with different strategies available.
- Undo and Redo: Ability to undo and redo moves during the game.
- Surrender: Option to surrender the game, declaring victory for the opponent.
- Possible moves are highlighted in grey.

### Prerequisites 
- Java Development Kit (JDK) installed on your system
- Here I do use Version 21, so if you use  a different one please update the pom.xml consequentially.

### What can be Improved
- I know the class Board is way too long... nearly 800 lignes.
- I tried to resume all the flipPiece and Verify functions in a shorter one, but unfortunately I wasn't able to get what I wanted and consequently I left them separate

#### Programmer
- Giuliano Taurone 
- Matricule : 59333