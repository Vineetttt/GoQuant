#ifndef MODIFY_ORDER_H
#define MODIFY_ORDER_H

#include <string>
#include <stdexcept>
#include "HTTPClient.h"
#include <nlohmann/json.hpp>

class ModifyOrder {
public:
    ModifyOrder(const std::string& access_token);

    // Only order_id, quantity, are required. Other parameters are optional.
    std::string modifyOrder(
        int64_t order_id,       
        int quantity,           
        double price=0,           
        bool post_only = false, 
        const std::string& adv = "",
        double stopPx = 0.0         
    );

private:
    std::string access_token;
    HttpClient http_client;

    void validateParameters(int64_t order_id, int quantity);
    std::string buildRequestUrl(int64_t order_id, int quantity, double price, bool post_only, const std::string& adv, double stopPx);
};

#endif
