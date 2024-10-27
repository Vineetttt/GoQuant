#include <iostream>
#include "DeribitAuth.h"
#include "PlaceOrder.h"
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
    std::string client_id = "nl4jIiYR"; 
    std::string client_secret = "-y7JN6FPCR7vkF864OJ-o78rpNxW8wn-g8D1q23ZoaE"; 

    try {
        // Authentication
        std::cout << "\nInitializing authentication..." << std::endl;
        DeribitAuth auth(client_id, client_secret);
        std::string auth_response = auth.authenticate();
        std::cout << "\nAuthentication Response:\n" << std::string(50, '-') << std::endl;
        prettyPrintJson(auth_response);

        auto json_auth = nlohmann::json::parse(auth_response);
        if (!json_auth.contains("result") || !json_auth["result"].contains("access_token")) {
            throw std::runtime_error("Failed to get access token");
        }

        std::cout << "\nInitializing trading..." << std::endl;
        PlaceOrder trading(auth.getAccessToken());

        // Place a buy order
        std::string order_response = trading.placeBuyOrder(
            "ETH-PERPETUAL", 
            10,              
            "market",       
            "market0000234"  
        );

        std::cout << "\nOrder Response:\n" << std::string(50, '-') << std::endl;
        prettyPrintJson(order_response);

        auto json_order = nlohmann::json::parse(order_response);
        if (json_order.contains("error")) {
            throw std::runtime_error("Order error: " + json_order["error"]["message"].get<std::string>());
        }

    } catch (const std::exception& e) {
        std::cerr << "Error occurred: " << e.what() << std::endl; 
        return 1;
    }

    return 0; 
}
