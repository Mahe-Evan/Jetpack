# Jetpack

## Description

Jetpack is a multiplayer game inspired by **Jetpack Joyride**, developed in **C/C++**.
The game uses a custom **TCP-based protocol**, allowing two players to compete simultaneously.

It consists of:
- **A server (`jetpack_server`)** that manages game logic and client communication.
- **A client (`jetpack_client`)** that handles game rendering and player input.

## Features

- **Custom network protocol** using `poll(2)`.
- **Multiplayer gameplay** with at least two players required to start.
- **Game elements** including maps, coins, and obstacles.
- **Server-controlled collisions** to prevent client-side cheating.
- **Graphical interface** using an authorized library (SFML, SDL, etc.).
- **Debug mode (`-d` flag)** for logging network packets.

## Installation

### Prerequisites
- A C/C++ compiler (e.g., `gcc` or `g++`).
- `make` to build the project.
- A supported graphical library (e.g., SFML, SDL).

### Compilation
Use the provided `Makefile` to build the project:

```sh
# Compile both client and server
make all

# Compile only the client
make client

# Compile only the server
make server
```

To clean compiled files:

```sh
make clean   # Remove object files
make fclean  # Remove binaries and object files
make re      # Clean and rebuild everything
```

## Usage

### Running the Server

```sh
./jetpack_server -p <port> -m <map_file> [-d]
```

- `<port>`: Port to listen on.
- `<map_file>`: Path to the game map file.
- `-d` (optional): Enable debug mode to log network packets.

Example:

```sh
./jetpack_server -p 4242 -m maps/level1.txt -d
```

### Running the Client

```sh
./jetpack_client -h <server_ip> -p <port> [-d]
```

- `<server_ip>`: IP address of the server.
- `<port>`: Port to connect to.
- `-d` (optional): Enable debug mode.

Example:

```sh
./jetpack_client -h 127.0.0.1 -p 4242 -d
```

## Documentation

- The **network protocol** and **map format** are fully documented in `doc.txt` (formatted as an RFC).
- Players can collect **coins** (score points) and must avoid **electric obstacles** (instant death).
- The game ends when:
  - A player reaches the end of the map.
  - A player dies (the other wins).
  - Both reach the end, and the one with the highest score wins.

## License

This project is for educational purposes and follows the Epitech project guidelines.
Sharing **code** between groups is forbidden, but **protocol documentation** can be shared for compatibility testing.
You can see the **license** in the [LICENSE](LICENSE) file.
