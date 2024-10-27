#ifndef PLACE_ORDER_H
#define PLACE_ORDER_H

#include <string>
#include <stdexcept>
#include "HTTPClient.h"
#include <nlohmann/json.hpp>

class PlaceOrder {
public:
    PlaceOrder(const std::string& access_token);

    // Only instrument and quantity are required. Others are optional.
    std::string placeBuyOrder(
        const std::string& instrument,
        int quantity,
        const std::string& order_type = "limit",
        const std::string& label = "",
        double price = 0.0 // Default to 0; should be used with limit/stop orders
    );

private:
    std::string access_token;
    HttpClient http_client;

    void validateParameters(const std::string& instrument,int quantity, const std::string& order_type);
    std::string buildRequestUrl(const std::string& instrument, int quantity, const std::string& order_type, const std::string& label, double price);
};

#endif