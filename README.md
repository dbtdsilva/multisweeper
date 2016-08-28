# Multisweeper
**Multisweeper** is a game where the objective consists in discovering the maximum numbers of mines as possible.

There is a 2D board with mines hidden on it. Each player (on their turn) will have the chance to reveal a position from the board; it might has a mine or a number on it. If it is a number, it means the total number of adjacent mines to that position. 

It is similar to Minesweeper but it is playable by several players and has other options, such as a special bomb (reveals 3x3 squares and can only be used once) that can be thrown. 

## Code description

The **logic** was separated from the **visual interface**. So it is possible to create several different interfaces for this engine. 
The logic is contained in the project "Multisweeper" which is implemented in C++ and can be easily ported to Linux (might do it later).
At the moment there is only a Console version implemented using PDCurses (more details can be found in the following section).

### Console version
This implementation was done using PDCurses (to support multiplatform since ncurses is only for Linux) and can be found in the project "MultisweeperConsole".

This visual interface example implements all the functionality previwed by the Engine. It means that it is possible to manipulate board, players and the game. The following image shows and example of a game with 4 players.

![alt tag](https://raw.githubusercontent.com/dbtdsilva/multisweeper/master/Samples/Console/board_finish.JPG)

More screenshots of this interface can be found in Samples/Console.

## More info

If you're having problems running this project, feel free to send me a message.
