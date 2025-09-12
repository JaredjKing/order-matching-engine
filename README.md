# Order Matching Engine

A **low-latency trading engine** implemented in C++20.  
Supports Market and Limit orders with a custom **skip list** for efficient storage and FIFO execution.  
Includes a lightweight **TCP order-entry protocol** and a **UDP multicast market data feed** to emulate real stock exchange behavior.  

---

## ✨ Features
- **Market & Limit Orders**: Deterministic matching logic with FIFO priority.  
- **Optimized Skip List**: O(log n) insert/delete with queue-based price levels.  
- **High-Performance Execution**: ~**500 ns** per match in benchmarks.  
- **TCP Order-Entry**: Clients can submit orders over sockets and receive acknowledgments in real time.  
- **UDP Multicast Market Data**: Trades and book deltas broadcast to subscribers, mimicking exchange feeds.  c
- **Deterministic Core**: Matching runs in a single-threaded engine; networking handled at the edges.  

---

## 🚀 Planned Enhancements
- Support for Stop and Stop-Limit orders.  
- Cancel support via TCP order-entry API.  
- Full depth-of-book multicast snapshots.  
- Multi-symbol and multi-threaded extension.  

---

## ⚙️ Setup
### Prerequisites
- C++20 or later  
- CMake ≥ 3.16  
- Linux (for sockets + multicast)  

### Building
```sh
git clone https://github.com/<your-username>/order-matching-engine.git
cd order-matching-engine
mkdir build && cd build
cmake ..
make -j
```

---

## ▶️ Usage
### Run in CSV Mode (backtest)
```sh
./order_matching_engine
```

### Run in Network Mode (exchange emulator)
```sh
./order_matching_engine --net
```
- **TCP Order Entry**: Connect on port `5000` and send line-delimited messages:  
  ```
  NEW|id=1|sym=SYM|side=B|px=100.5|qty=50
  CANCEL|id=1
  ```
- **UDP Market Data**: Subscribe to multicast group `239.0.0.1:30000` to receive trades/book updates.  

Example listener:
```sh
socat -u UDP4-RECVFROM:30000,ip-add-membership=239.0.0.1:0.0.0.0 STDOUT
```

---

## 📂 Project Structure
- `src/OrderBook.cpp` – core matching logic (skip list + FIFO).  
- `src/TcpServer.cpp` – TCP order-entry gateway.  
- `src/UdpPublisher.cpp` – UDP multicast publisher for trades/book deltas.  
- `include/` – headers for modular components.  

---

## 🤝 Contributions
Contributions are welcome!  
Open an issue or submit a PR if you’d like to add features, optimizations, or exchange-style protocols.  
