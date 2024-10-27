#ifndef CANCEL_ORDER_H
#define CANCEL_ORDER_H

#include <string>
#include <stdexcept>
#include "HTTPClient.h"
#include <nlohmann/json.hpp>

class CancelOrder {
public:
    CancelOrder(const std::string& access_token);

    std::string cancelOrder(const std::string& order_id);

private:
    std::string access_token;
    HttpClient http_client;
};

#endif
