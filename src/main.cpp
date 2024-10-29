#include "DeribitTrading.h"
#include "WebSocketServer.h"
#include <iostream>

int main() {
    std::string client_id = "nl4jIiYR";
    std::string client_secret = "-y7JN6FPCR7vkF864OJ-o78rpNxW8wn-g8D1q23ZoaE";
    
    try {
        DeribitTrading trading(client_id, client_secret);
        
        if (!trading.authenticate()) {
            return 1;
        }

        trading.placeOrder("BTC-PERPETUAL", 10, "limit", "market0000234", 2000.0);
        trading.cancelOrder("29208640589");
        trading.modifyOrder(29213881074, 100);
        trading.getOrderBook("ETH-PERPETUAL", 1);
        trading.getPosition("ETH-PERPETUAL");

        DeribitWebSocketServer server;
        server.start(9002);

    } catch (const std::exception& e) {
        Logger::getInstance()->log("Critical error: " + std::string(e.what()), "CRITICAL");
        return 1;
    }
    
    return 0;
}