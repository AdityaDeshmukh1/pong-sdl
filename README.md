# Pong-SDL: LAN Multiplayer Pong in C

A simple classic Pong game built in C using SDL2 and SDL2_ttf, featuring local multiplayer over LAN using TCP sockets.


## Features

- Single-player mode against AI
- Local multiplayer on the same machine
- **LAN multiplayer mode (host-client model)**
- Real-time ball and paddle physics
- SDL2-based rendering, input handling, and text display


## Dependencies

Tested on **Linux** (Fedora/Ubuntu/Arch)

### Install SDL2 and SDL2_ttf:
#### On Debian/Ubuntu:
```bash
sudo apt update
sudo apt install libsdl2-dev libsdl2-ttf-dev
```

#### On Fedora:
```bash
sudo dnf install SDL2-devel SDL2_ttf-devel
```

#### On Arch/Manjaro:
```bash
sudo pacman -S sdl2 sdl2_ttf
```


## Build Instructions

Clone the repository:
```bash
git clone https://github.com/AdityaDeshmukh1/pong-sdl.git
cd pong-sdl
```

Compile the game:
```bash
make
```

## Next Planned Features

- [ ] **Implement non-blocking sockets** using `fcntl()` or `select()` for smoother real-time multiplayer  
- [ ] **Improve AI opponent behavior**
- [ ] **Add latency/ping display on screen**
- [ ] **Optional sound effects with SDL2_mixer**
- [ ] **Add support for multiple clients (expand to 2v2 or spectators)**
- [ ] **Clean modularized network code structure (`network_init()`, `network_poll()`, `network_shutdown()`)**
- [ ] **Frame-rate independent physics**
- [ ] **Network lag compensation / interpolation**

## Learnings / Concepts Covered

- SDL2 rendering, input handling, and text rendering  
- Structuring a C project with modular `.c` and `.h` files  
- TCP socket programming in C on Linux  
- LAN-based multiplayer game architecture  
- Debugging low-level networking issues (bad file descriptors, blocking calls, connection resets)  
- System-level debugging tools (`ip addr`, `ping`, `ss`, `ufw`, `firewalld`)  

## License

MIT License 

## Author

Aditya Deshmukh  
*Systems Programming, Networking, and Game Programming Enthusiast*
