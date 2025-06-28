# Pong-SDL: LAN Multiplayer Pong in C

A simple classic Pong game built in C using SDL2 and SDL2_ttf, featuring local multiplayer over LAN using TCP sockets.

---

## 📌 Features

- Single-player mode against AI
- Local multiplayer on the same machine
- **LAN multiplayer mode (host-client model)**
- Real-time ball and paddle physics
- SDL2-based rendering, input handling, and text display

---

## 📦 Dependencies

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

---

## 🔧 Build Instructions

Clone the repository:
```bash
git clone https://github.com/yourname/pong-sdl.git
cd pong-sdl
```

Compile the game:
```bash
gcc -o pong main.c network.c init.c game.c render.c input.c menu.c ai.c -lSDL2 -lSDL2_ttf
```

---

## 🎮 Running the Game

### Single-player:
```bash
./pong
```

---

### LAN Multiplayer:

#### 1️⃣ On the Host Laptop:
```bash
./pong_serv
```

#### 2️⃣ On the Client Laptop (replace `<host_ip>` with actual IP):
```bash
./pong_client <host_ip>
```

Get the host IP by running:
```bash
ip addr show
```
and looking for your active interface's `inet` address (like `192.168.0.45`)

**Note:**  
Make sure both machines are connected to the same Wi-Fi or LAN network and firewall ports are open for TCP port `8080`.

---

## 📈 Next Planned Features

- [ ] **Decouple input and game state packets** to fix paddle desync and reduce network lag  
- [ ] **Implement non-blocking sockets** using `fcntl()` or `select()` for smoother real-time multiplayer  
- [ ] **Improve AI opponent behavior**
- [ ] **Add menu option for multiplayer IP and port entry**
- [ ] **Add latency/ping display on screen**
- [ ] **Optional sound effects with SDL2_mixer**
- [ ] **Add support for multiple clients (expand to 2v2 or spectators)**
- [ ] **Clean modularized network code structure (`network_init()`, `network_poll()`, `network_shutdown()`)**
- [ ] **Frame-rate independent physics**
- [ ] **Network lag compensation / interpolation**

---

## 📖 Learnings / Concepts Covered

- SDL2 rendering, input handling, and text rendering  
- Structuring a C project with modular `.c` and `.h` files  
- TCP socket programming in C on Linux  
- LAN-based multiplayer game architecture  
- Debugging low-level networking issues (bad file descriptors, blocking calls, connection resets)  
- System-level debugging tools (`ip addr`, `ping`, `ss`, `ufw`, `firewalld`)  

---

## 📜 License

MIT License (or your preferred license)

---

## 📣 Author

Aditya Deshmukh  
*Systems Programming, Networking, and Game Programming Enthusiast*
