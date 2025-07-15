# 🪙 MiniTrader

> A lightweight, scalable multi-client trading system built in modern C++17  
> featuring event-driven design (using kqueue on macOS), modular architecture, and real-time order book management.

---

## 🚀 Features

- 📡 Multi-client support via single-threaded `kqueue` event loop
- 👤 Clients identify themselves with a username
- ✅ Process real-time trading commands: `BUY`, `SELL`, etc.
- 📚 Central `OrderBook` keeps track of all orders
- 🛠 Admin thread on server side for runtime commands (e.g., `print orderbook`, `exit`)
- 🧩 Modular C++ design: `Trader`, `Order`, `OrderBook`, `TradingEngine`
- ⚙ Built with modern C++17

---
<img width="1377" height="518" alt="image" src="https://github.com/user-attachments/assets/b074277d-70f6-472d-a342-8e5d769b5dd2" />


## 🗂️ Project structure

```plaintext
trading-system/
├── include/
│   ├── Order.hpp
│   ├── OrderBook.hpp
│   ├── Trader.hpp
│   └── TradingEngine.hpp
├── src/
│   ├── OrderBook.cpp
│   ├── TradingEngine.cpp
│   └── server.cpp
├── CMakeLists.txt
└── README.md
```
## ⚙️ Setup, Build & Run

### ✅ Requirements

- macOS  
- CMake ≥ 3.10  
- Clang++ (from Xcode Command Line Tools) or any compiler supporting C++17

---

### 🛠 Setup & Build

Clone the repository and create a build directory:
```bash
git clone https://github.com/yourusername/trading-system.git
cd trading-system

mkdir build
cd build

cmake ..
make
```

### 🛠 Run
Run server and client
```bash
./server
./client
```
