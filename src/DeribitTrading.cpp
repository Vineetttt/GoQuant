#include <iostream>
#include "Logger.h"
#include "DeribitTrading.h"
#include "DeribitAuth.h"
#include "PlaceOrder.h"
#include "CancelOrder.h"
#include "ModifyOrder.h"
#include "GetOrderBook.h"
#include "GetPosition.h"
#include <nlohmann/json.hpp>
#include "WebSocketServer.h"
#include <thread>

// Implementation of DeribitTrading methods
DeribitTrading::DeribitTrading(const std::string& id, const std::string& secret) 
    : client_id(id), client_secret(secret) {
    logger = Logger::getInstance();
}

bool DeribitTrading::authenticate() {
    try {
        logger->log("Initiating authentication...");
        DeribitAuth auth(client_id, client_secret);
        std::string auth_response = auth.authenticate();
        logger->logJson("Authentication Response", auth_response);

        auto json_auth = nlohmann::json::parse(auth_response);
        if (!json_auth.contains("result") || !json_auth["result"].contains("access_token")) {
            throw std::runtime_error("Failed to get access token");
        }

        access_token = json_auth["result"]["access_token"];
        logger->log("Authentication successful");
        return true;
    } catch (const std::exception& e) {
        logger->log("Authentication failed: " + std::string(e.what()), "ERROR");
        return false;
    }
}

bool DeribitTrading::placeOrder(const std::string& instrument, double quantity, 
                                 const std::string& order_type, const std::string& label, double price) {
    try {
        logger->log("Placing order...");
        PlaceOrder trading(access_token);
        std::string order_response = trading.placeBuyOrder(instrument, quantity, order_type, label, price);
        logger->logJson("Order Response", order_response);

        auto json_order = nlohmann::json::parse(order_response);
        if (json_order.contains("error")) {
            throw std::runtime_error("Order error: " + json_order["error"]["message"].get<std::string>());
        }
        return true;
    } catch (const std::exception& e) {
        logger->log("Order placement failed: " + std::string(e.what()), "ERROR");
        return false;
    }
}

bool DeribitTrading::cancelOrder(const std::string& order_id) {
    try {
        logger->log("Cancelling order " + order_id);
        CancelOrder cancelOrder(access_token);
        std::string cancel_response = cancelOrder.cancelOrder(order_id);
        logger->logJson("Cancel Response", cancel_response);
        return true;
    } catch (const std::exception& e) {
        logger->log("Order cancellation failed: " + std::string(e.what()), "ERROR");
        return false;
    }
}

bool DeribitTrading::modifyOrder(int64_t order_id, int new_quantity) {
    try {
        logger->log("Modifying order " + std::to_string(order_id));
        ModifyOrder modifyOrder(access_token);
        std::string modify_response = modifyOrder.modifyOrder(order_id, new_quantity);
        logger->logJson("Modify Response", modify_response);
        return true;
    } catch (const std::exception& e) {
        logger->log("Order modification failed: " + std::string(e.what()), "ERROR");
        return false;
    }
}

bool DeribitTrading::getOrderBook(const std::string& instrument_name, int depth) {
    try {
        logger->log("Fetching order book for " + instrument_name);
        GetOrderBook orderBook;
        std::string orderbook_response = orderBook.getOrderBook(instrument_name, depth);
        logger->logJson("OrderBook Response", orderbook_response);
        return true;
    } catch (const std::exception& e) {
        logger->log("Order book fetch failed: " + std::string(e.what()), "ERROR");
        return false;
    }
}

bool DeribitTrading::getPosition(const std::string& instrument_name) {
    try {
        logger->log("Fetching position for " + instrument_name);
        GetPosition getPosition(access_token);
        std::string position_response = getPosition.getPosition(instrument_name);
        logger->logJson("Position Response", position_response);
        return true;
    } catch (const std::exception& e) {
        logger->log("Position fetch failed: " + std::string(e.what()), "ERROR");
        return false;
    }
}