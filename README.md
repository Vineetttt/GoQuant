# Deribit Order Management System

This is a C++ application designed for order execution and management on the Deribit Test environment. The system provides various functions to place, cancel, and modify orders, view the orderbook and current positions, and supports both REST and WebSocket connections for real-time data streaming.

## Features

- **Place Order**: Submit a new order to the Deribit Test API.
- **Cancel Order**: Cancel an existing order.
- **Modify Order**: Modify details of an existing order.
- **Get Orderbook**: Retrieve the orderbook for a specified symbol.
- **View Current Positions**: Check the user's open positions on Deribit.
- **WebSocket Server** (Advanced Bonus): Allows clients to connect and subscribe to symbols, receiving real-time orderbook updates.

## Requirements

- **C++17** or later
- **CMake 3.10** or later
- **Libraries**:
  - `libcurl`
  - `nlohmann_json`
  - `Boost (system component)`
  - `websocketpp`

## Installation

Clone the repository and navigate into the project directory:

```bash
git clone [https://github.com/yourusername/DeribitOrderManagementSystem.git](https://github.com/Vineetttt/GoQuant.git)
```
```bash
cd DeribitOrderManagementSystem
```
```bash
mkdir build && cd build
```
```bash
cmake ..
```
```bash
make
```
```bash
./DeribitOrderManagementSystem
```

