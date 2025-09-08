#!/usr/bin/env python3
"""
Tic-tac-toe game for two players
Features:
- Game board display
- Player turns
- Win condition checking
- Simple command-line interface
"""


class TicTacToe:
    def __init__(self):
        # Initialize 3x3 board with empty spaces
        self.board = [[' ' for _ in range(3)] for _ in range(3)]
        self.current_player = 'X'
    
    def display_board(self):
        """Display the current state of the game board"""
        print("\n   0   1   2")
        for i in range(3):
            print(f"{i}  {self.board[i][0]} | {self.board[i][1]} | {self.board[i][2]}")
            if i < 2:
                print("  -----------")
        print()
    
    def is_valid_move(self, row, col):
        """Check if the move is valid (within bounds and cell is empty)"""
        return 0 <= row < 3 and 0 <= col < 3 and self.board[row][col] == ' '
    
    def make_move(self, row, col):
        """Make a move on the board"""
        if self.is_valid_move(row, col):
            self.board[row][col] = self.current_player
            return True
        return False
    
    def check_winner(self):
        """Check if there's a winner"""
        # Check rows
        for row in self.board:
            if row[0] == row[1] == row[2] != ' ':
                return row[0]
        
        # Check columns
        for col in range(3):
            if self.board[0][col] == self.board[1][col] == self.board[2][col] != ' ':
                return self.board[0][col]
        
        # Check diagonals
        if self.board[0][0] == self.board[1][1] == self.board[2][2] != ' ':
            return self.board[0][0]
        if self.board[0][2] == self.board[1][1] == self.board[2][0] != ' ':
            return self.board[0][2]
        
        return None
    
    def is_board_full(self):
        """Check if the board is full (tie game)"""
        for row in self.board:
            for cell in row:
                if cell == ' ':
                    return False
        return True
    
    def switch_player(self):
        """Switch to the other player"""
        self.current_player = 'O' if self.current_player == 'X' else 'X'
    
    def get_player_input(self):
        """Get and validate player input"""
        while True:
            try:
                print(f"Player {self.current_player}'s turn")
                row = int(input("Enter row (0-2): "))
                col = int(input("Enter column (0-2): "))
                return row, col
            except ValueError:
                print("Invalid input! Please enter numbers only.")
            except KeyboardInterrupt:
                print("\nGame interrupted. Goodbye!")
                exit(0)
    
    def play(self):
        """Main game loop"""
        print("Welcome to Tic-tac-toe!")
        print("Players take turns placing X and O on a 3x3 grid.")
        print("Enter row and column coordinates (0-2) to make your move.")
        
        while True:
            self.display_board()
            
            # Get player move
            row, col = self.get_player_input()
            
            # Validate and make move
            if not self.make_move(row, col):
                print("Invalid move! That position is already taken or out of bounds.")
                continue
            
            # Check for winner
            winner = self.check_winner()
            if winner:
                self.display_board()
                print(f"🎉 Player {winner} wins!")
                break
            
            # Check for tie
            if self.is_board_full():
                self.display_board()
                print("It's a tie! The board is full.")
                break
            
            # Switch to other player
            self.switch_player()


def main():
    """Main function to start the game"""
    while True:
        game = TicTacToe()
        game.play()
        
        # Ask if players want to play again
        while True:
            play_again = input("\nDo you want to play again? (y/n): ").lower().strip()
            if play_again in ['y', 'yes']:
                break
            elif play_again in ['n', 'no']:
                print("Thanks for playing! Goodbye!")
                return
            else:
                print("Please enter 'y' for yes or 'n' for no.")


if __name__ == "__main__":
    main()