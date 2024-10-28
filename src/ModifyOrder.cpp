#include "ModifyOrder.h"
#include <stdexcept>

ModifyOrder::ModifyOrder(const std::string& access_token)
    : access_token(access_token) {}

void ModifyOrder::validateParameters(int64_t order_id, int quantity) {
    if (order_id <= 0) {
        throw std::invalid_argument("Order ID must be a positive integer.");
    }
    if (quantity <= 0) {
        throw std::invalid_argument("Quantity must be greater than zero.");
    }
}

std::string ModifyOrder::buildRequestUrl(int64_t order_id, int quantity, double price, bool post_only, const std::string& adv, double stopPx) {
    const std::string url = "https://test.deribit.com/api/v2/private/edit";

    std::string request_url = url + "?order_id=" + std::to_string(order_id) +
                               "&amount=" + std::to_string(quantity);


    if(price > 0){
        request_url += "&price="+std::to_string(price);
    }
    if (post_only) {
        request_url += "&post_only=true";
    }
    if (!adv.empty()) {
        request_url += "&adv=" + adv;
    }
    if (stopPx > 0) {
        request_url += "&stopPx=" + std::to_string(stopPx);
    }

    return request_url;
}

std::string ModifyOrder::modifyOrder(int64_t order_id, int quantity, double price, bool post_only, const std::string& adv, double stopPx) {
    // Validate required parameters
    validateParameters(order_id, quantity);

    // Build the request URL with optional parameters
    std::string request_url = buildRequestUrl(order_id, quantity, price, post_only, adv, stopPx);

    // Set headers
    std::vector<std::string> headers = {
        "Authorization: Bearer " + access_token,
        "Content-Type: application/json"
    };

    return http_client.get(request_url, headers); 
}
