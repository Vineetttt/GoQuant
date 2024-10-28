#include <iostream>
#include "DeribitAuth.h"
#include "PlaceOrder.h"
#include "CancelOrder.h"
#include "ModifyOrder.h"
#include "GetOrderBook.h"
#include "GetPosition.h"
#include <nlohmann/json.hpp>

void prettyPrintJson(const std::string& jsonStr) {
    try {
        auto j = nlohmann::json::parse(jsonStr);
        std::cout << j.dump(4) << std::endl; 
    } catch (const std::exception& e) {
        std::cout << "Raw response: " << jsonStr << std::endl; 
    }
}

int main() {
    std::string client_id = "gezprJoG"; 
    std::string client_secret = "BEImeE6CA_L2FbYQJDs76PrzN9DM6RsC5e57Nw7bnjc"; 

    try {
        
        // Authentication
        std::cout << "\nInitializing authentication..." << std::endl;
        DeribitAuth auth(client_id, client_secret);
        std::string auth_response = auth.authenticate();
        prettyPrintJson(auth_response);

        auto json_auth = nlohmann::json::parse(auth_response);
        if (!json_auth.contains("result") || !json_auth["result"].contains("access_token")) {
            throw std::runtime_error("Failed to get access token");
        }
        
        PlaceOrder trading(auth.getAccessToken());

        // Hardcoded values for placing an order
        std::string instrument = "BTC-PERPETUAL";
        double quantity = 10;
        std::string order_type = "limit";
        std::string label = "market0000234";
        double price = 2000.0; // Only applicable for limit and stop orders


        // Place an order
        std::string order_response = trading.placeBuyOrder(instrument, quantity, order_type, label, price);
        prettyPrintJson(order_response);

        auto json_order = nlohmann::json::parse(order_response);
        if (json_order.contains("error")) {
            throw std::runtime_error("Order error: " + json_order["error"]["message"].get<std::string>());
        }
        
        CancelOrder cancelOrder(auth.getAccessToken());
        std::string cancellation_response = cancelOrder.cancelOrder("29208640589");
        prettyPrintJson(cancellation_response);

        ModifyOrder modifyOrder(auth.getAccessToken());

        int64_t order_id = 29213881074;
        int quantity_new = 100;

        std::string modify_response = modifyOrder.modifyOrder(order_id, quantity_new);
        prettyPrintJson(modify_response);

        GetOrderBook orderBook;
        std::string instrument_name = "ETH-PERPETUAL"; 
        int depth = 1; 

        std::string order_book_response = orderBook.getOrderBook(instrument_name, depth);
        prettyPrintJson(order_book_response);

        // Retrieve position
        GetPosition getPosition(auth.getAccessToken());
        std::string name = "ETH-PERPETUAL"; // Specify the instrument name
        std::string position_response = getPosition.getPosition(name);
        prettyPrintJson(position_response);
       
    } catch (const std::exception& e) {
        std::cerr << "Error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
