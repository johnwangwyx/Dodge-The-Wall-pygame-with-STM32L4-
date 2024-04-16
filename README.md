# Dodge the Wall - Flight Simulator

"Dodge the Wall - Flight Simulator" is an interactive game where players control a virtual aircraft through physical hand movements, detected by an STM32 board's accelerometer. The game environment and aircraft responses are displayed and managed through a Python-based GUI.

## DEMO
Demo play can be found here (You can turn on the sound to see the BGM and sound effects):

https://github.com/johnwangwyx/Dodge-The-Wall-pygame-with-STM32L4-/assets/78456315/c2cbe2b2-f547-4a0d-ada6-3bed026dd8c4

It is quite fun, please check it out :)

## Features
- **Real-Time Aircraft Control**: Tilt the STM32 board to control the aircraft's movement in the game.
- **Dynamic Obstacles**: Avoid dynamically generated walls that move towards your aircraft with increasing obstacle speed.
- **Sound Effects and Background Music**: Experience real-time audio feedback for actions such as collecting coins or crashing from your STM32 board (speaker needed on your board).
- **Leaderboard**: Compete with others by trying to achieve the highest scores and best survival times, stored and retrieved from the STM32's memory.

## Prerequisites
To run the game, you will need:
- Python 3.x installed on your computer.
- Pygame library installed (`pip install pygame`).
- PySerial library installed for serial communication (`pip install pyserial`).

For the hardware part, you will require:
- STM32L475 IoT node.
- Micro USB to USB cable for communication.
- The STM32 board must be programmed with the provided `main.c` logic.

## Game Setup
1. **Clone the Repository**: Clone this repo to your local machine.
2. **Connect the STM32 Board**: Connect the STM32 board to your computer via USB.
3. **Load the STM32 Firmware**: Flash the `main.c` program onto the STM32 board using STM32CubeIDE or a similar tool.
4. **Run the Game**: Navigate to the repository's directory and run `python game.py` to start the game.

## Controls
- **Tilt the STM32 Board**: Tilt the board left or right to move the aircraft horizontally, and tilt forward or backward to move vertically.

## Hardware Interaction
- The STM32 board sends accelerometer data to the Python game via UART, controlling the on-screen aircraft based on the board's orientation.
- Sound effects for game events (e.g., coin collection and explosions) are managed through the DAC outputs of the STM32.

## Game Flow
- The game begins at a start screen, waiting for the player to press the space bar.
- As the game progresses, walls appear at the top of the screen and move downwards.
- The player must avoid these walls by moving the aircraft, using the STM32 board's tilt angles.
- Coins appear periodically which the player can collect for points.
- The game tracks the duration of survival and the number of coins collected.

## Code Structure
### Python (game.py)
- Handles game graphics, collision detection, and GUI interactions.
- Receives tilt data from the STM32 board to control the aircraft.

### C (main.c)
- Manages accelerometer data reading, sound effects via DAC, and UART communication.
- Keeps track of high scores and survival times using onboard QSPI memory.

