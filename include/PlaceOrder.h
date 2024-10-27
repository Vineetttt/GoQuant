#ifndef PLACE_ORDER_H
#define PLACE_ORDER_H

#include <string>
#include <stdexcept>
#include "HTTPClient.h"
#include <nlohmann/json.hpp>

class PlaceOrder {
public:
    PlaceOrder(const std::string& access_token);
    
    std::string placeBuyOrder(
        const std::string& instrument_name,
        double quantity,
        const std::string& order_type = "market",
        const std::string& label = ""
    );

private:
    std::string access_token;
    HttpClient http_client;
};

#endif