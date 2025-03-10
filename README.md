# Order Matching Engine

This is a high-performance order matching engine implemented in C++. It supports Market and Limit orders with a **skip list** for efficient order storage and execution. Future improvements will include Stop and Stop-Limit orders, as well as multithreading for handling multiple tickers.

## Features
- **Market and Limit Orders**: Fully implemented matching logic.
- **Optimized Skip List**: Ensures efficient order retrieval while maintaining FIFO execution within price levels.
- **High-Performance Execution**: Orders are executed in **900ns**, making the engine suitable for low-latency trading.
- **Optimizations Used**:
  - TBD
- **Planned Enhancements**:
  - Support for Stop and Stop-Limit orders.
  - Further performance optimizations.

## Setup
### Prerequisites
- C++20 or later
- CMake
- Git

### Building
```sh
mkdir build && cd build
cmake ..
make
```

## Usage
Execute the compiled binary to start the order matching engine.
```sh
./order_matching_engine
```

## Contributions
Contributions are welcome! Feel free to open an issue or submit a pull request.
