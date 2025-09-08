# Tic-tac-toe Game

A simple command-line Tic-tac-toe game for two players implemented in Python.

## Features

- **Game board display**: Clear 3x3 grid with coordinate system
- **Player turns**: Alternating turns between X and O players
- **Win condition checking**: Detects wins in rows, columns, and diagonals
- **Input validation**: Handles invalid inputs and occupied positions
- **Command-line interface**: Simple text-based interface
- **Play again option**: Option to start a new game after completion

## How to Play

1. Run the game:
   ```bash
   python3 tictactoe.py
   ```

2. Players take turns entering coordinates:
   - Enter row number (0-2)
   - Enter column number (0-2)

3. The game will:
   - Display the updated board after each move
   - Check for wins or ties
   - Announce the winner or declare a tie
   - Ask if you want to play again

## Game Rules

- Player X always goes first
- Players alternate turns
- First player to get 3 in a row (horizontally, vertically, or diagonally) wins
- If all 9 squares are filled without a winner, it's a tie

## Example Game Board

```
   0   1   2
0  X |   | O
  -----------
1    | X |  
  -----------
2  O |   | X
```

## Requirements

- Python 3.x