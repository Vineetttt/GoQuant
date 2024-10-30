#include "DeribitTrading.h"
#include "WebSocketServer.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>

int main() {
    std::string client_id = std::getenv("DERIBIT_CLIENT_ID");
    std::string client_secret = std::getenv("DERIBIT_CLIENT_SECRET");
    
    try {
        DeribitTrading trading(client_id, client_secret);
        
        if (!trading.authenticate()) {
            return 1;
        }

        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        for (int i = 0; i < 20; i++) {
            threads.emplace_back([&trading, &i]() {
                trading.placeOrder("SOL_USDC-PERPETUAL", 10, "limit", "market0000" + std::to_string(i), 2.0);
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::cout << "Time taken to place 20 orders: " << duration << " ms" << std::endl;

        /*
        trading.placeOrder("ETH_PERPETUAL", 10, "limit", "market000012", 2.0);
        trading.cancelOrder("29208640589");
        trading.modifyOrder(29213881074, 100);
        trading.getOrderBook("ETH-PERPETUAL", 1);
        trading.getPosition("ETH-PERPETUAL");
        */

        /*
        DeribitWebSocketServer server;
        server.start(9002);
        */

    } catch (const std::exception& e) {
        Logger::getInstance()->log("Critical error: " + std::string(e.what()), "CRITICAL");
        return 1;
    }
    
    return 0;
}