#include "CancelOrder.h"
#include <stdexcept>

CancelOrder::CancelOrder(const std::string& access_token) 
    : access_token(access_token) {}

std::string CancelOrder::cancelOrder(const std::string& order_id) {
    const std::string url = "https://test.deribit.com/api/v2/private/cancel";

    // Create the URL with query parameters
    std::string request_url = url + "?order_id=" + order_id;

    // Define headers for the request
    std::vector<std::string> headers = {
        "Authorization: Bearer " + access_token,
        "Content-Type: application/json"
    };

    // Send the GET request and get the response
    return http_client.get(request_url, headers);
}
