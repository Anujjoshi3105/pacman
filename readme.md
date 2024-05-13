# Pacman Maze Game

Welcome to the Pacman Maze Game! This classic maze adventure allows you to guide Pacman through a challenging labyrinth while avoiding pesky ghosts.


![pacman](pacman.gif)

## Getting Started

Follow these simple steps to set up and run the game on your local machine.

### Prerequisites

Ensure you have the following installed on your system:

- C compiler (e.g., GCC)
- NCurses library ([Installation Guide](https://invisible-island.net/ncurses/))

### Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/anujjoshi3105/pacman.git
   ```

2. Compile the source code:

   ```bash
   gcc -o pacman pacman.c -lncurses
   ```

### Running the Game

Execute the compiled executable:

```bash
./pacman <map_file_name>
```

Replace `<map_file_name>` with the name of the text file containing the maze metadata.

## Game Controls

- Use arrow keys (UP/W, DOWN/S, LEFT/A, RIGHT/D) to move Pacman.
- Press 'U' to undo the last move.
- Press 'Z' to quit the game.

## Game Rules

Navigate Pacman through the maze to eat all pellets and reach the goal. Avoid ghosts roaming the maze; colliding with a ghost results in game over. Use the undo feature ('U') to backtrack and avoid ghosts effectively.

For detailed rules and specifications, refer to the [rule.pdf](/rule.pdf) document provided in the repository.

## Built With

- [NCurses](https://invisible-island.net/ncurses/) - Library for text-based user interfaces

## Authors

- **Anuj Joshi** - [GitHub Profile](https://github.com/anujjoshi3105)

## License

This project is licensed under the MIT License - see the [LICENSE](/LICENSE) file for details.

## Acknowledgments

- [NCurses Tutorial by Casual Coder](https://www.youtube.com/playlist?list=PL2U2TQ__OrQ8jTf0_noNKtHMuYlyxQl4v)
- [Pacman Game in C - GFG Blog](https://www.geeksforgeeks.org/pacman-game-in-c/)
