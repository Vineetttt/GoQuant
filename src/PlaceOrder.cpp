#include "PlaceOrder.h"
#include <stdexcept>

PlaceOrder::PlaceOrder(const std::string& access_token) 
    : access_token(access_token) {}

std::string PlaceOrder::placeBuyOrder(
    const std::string& instrument_name,
    double quantity,
    const std::string& order_type,
    const std::string& label
) {
    const std::string url = "https://test.deribit.com/api/v2/private/buy";

    // Create the URL with query parameters
    std::string request_url = url + "?amount=" + std::to_string(quantity) +
                               "&instrument_name=" + instrument_name +
                               "&label=" + label +
                               "&type=" + order_type;

    std::vector<std::string> headers = {
        "Authorization: Bearer " + access_token,
        "Content-Type: application/json"
    };

    return http_client.get(request_url, headers); 
}
