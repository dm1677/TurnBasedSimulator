# TurnBasedSimulator

Reimplementation of Turn Based game logic in C++ for use with AI.

## Features

### AI
There are 5 types of AI currently implemented:
-Minimax
-Monte Carlo
-Monte Carlo Tree Search
-Random (uniform distribution)
-Random (weighted distribution)

The Monte Carlo AIs use the Random AI for their random simulations (rollouts), and can choose which version of the Random AI to use.
A very simple evaluation function calculates the difference between the combined health of both player's kings, normalised between 0 and 1, as a heuristic. Reducing your opponent's king's health to 0 is the victory condition, making this a very suitable heuristic.
The random simulation phase of the Monte Carlo AIs uses multithreading to perform several simulations in parallel. This is the simplest, but perhaps not the most effective way to parallelise them.

### Puzzles
Similar to Chess puzzles, the simulator can read and write puzzle files which contain a single game state, and one or more actions (correct solutions).

### Testing
The simulator can use puzzles to test the different AIs by comparing the action that the AI chooses with the predetermined correct action.
Thus, a robust and error-free set of tests is vital in ensuring any testing data is valid.
As all of the effective AIs are stochastic, the simluator is capable of running the tests any number of times.
This allows for comparison of the performance of different AIs against a consistent set of test cases, which makes understanding their behaviour easier.
Testing also records the amount of time taken to complete each test instance.

The simulator can also play two different AIs against one another directly and record statistics about their game.

### Replays
As in the original Turn Based, the simulator can save and load replay files of matches. Currently there is no cross-compatibility between the two game versions.

### Rendering
The game state can be printed to the console or drawn to a window using the graphics from the original game. This rendering is multithreaded such that the AI computations happen in parallel and don't block the rendering.

## Game State Representation
The game state is represented by a list of units; this gives the most memory-efficient implementation, not wasting memory on empty cells on the game board.
Each unit is represented using 21 bits. 
Given a 15x15 game board, there is a maximum of 225 units per state. The resource count of each player is also encoded as a unit. Some extra data is required to encode which player's turn it is.
Assuming each unit is packed into a 32-bit integer, this gives a rough upper bound of 1kb per state.

Furthermore, some access operations to attributes of the game state can be optimised by using knowlege of the order of the list.
For instance, the resources and king units are always in the first positions in the list, meaning no iteration is required to read the resource count or perform game-over checks.

This representation strategy was devised to improve the performance of the minimax algorithm, and was the main motivator for choosing to reimplement the game logic in C++. The AI's performance in the original game in C# was inadequate.
Despite this, due to the large branching factor and search depth of the game, the naive minimax approach was abandoned in favour of MCTS.

Other representation strategies are being investigated (see BitBoard branch).

