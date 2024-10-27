#include <iostream>
#include <cstdlib>
#include "DeribitAuth.h"
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
    // Fetch client ID and secret from environment variables
    std::string client_id = "nl4jIiYR"; 
    std::string client_secret = "-y7JN6FPCR7vkF864OJ-o78rpNxW8wn-g8D1q23ZoaE";

    // Check if the variables are set
    if (client_id.empty() || client_secret.empty()) {
        std::cerr << "Error: Please set your CLIENT_ID and CLIENT_SECRET environment variables" << std::endl;
        return 1;
    }

    try {
        std::cout << "\nInitializing WebSocket connection..." << std::endl;
        DeribitAuth auth(client_id, client_secret);
        
        std::cout << "Attempting authentication..." << std::endl;
        std::string response = auth.authenticate();
        
        std::cout << "\nAuthentication Response:\n" << std::string(50, '-') << std::endl;
        prettyPrintJson(response);
        std::cout << std::string(50, '-') << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}