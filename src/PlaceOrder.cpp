#include "PlaceOrder.h"
#include <stdexcept>

PlaceOrder::PlaceOrder(const std::string& access_token)
    : access_token(access_token) {}

void PlaceOrder::validateParameters(const std::string& instrument, int quantity, const std::string& order_type) {
    if (instrument.empty()) {
        throw std::invalid_argument("Instrument name cannot be empty.");
    }
    if (quantity <= 0) {
        throw std::invalid_argument("Quantity must be greater than zero.");
    }
    if (order_type != "limit" && order_type != "stop_limit" && order_type != "market") {
        throw std::invalid_argument("Invalid order type. Valid types are: limit, stop_limit, market.");
    }
}

std::string PlaceOrder::buildRequestUrl(const std::string& instrument, int quantity, const std::string& order_type, const std::string& label, double price) {
    const std::string url = "https://test.deribit.com/api/v2/private/buy";

    // Construct the request URL with query parameters
    std::string request_url = url + "?instrument_name=" + instrument +
                               "&amount=" + std::to_string(quantity) +
                               "&type=" + order_type;

    if (!label.empty()) {
        request_url += "&label=" + label;
    }
    if ((order_type == "limit" || order_type == "stop_limit") && price > 0) {
        request_url += "&price=" + std::to_string(price);
    }

    return request_url;
}

std::string PlaceOrder::placeBuyOrder(
    const std::string& instrument,
    int quantity,
    const std::string& order_type,
    const std::string& label,
    double price
) {
    // Validate the parameters before proceeding
    validateParameters(instrument, quantity, order_type);

    // Build the request URL
    std::string request_url = buildRequestUrl(instrument, quantity, order_type, label, price);

    // Set headers
    std::vector<std::string> headers = {
        "Authorization: Bearer " + access_token,
        "Content-Type: application/json" // May not be needed for GET requests
    };

    return http_client.get(request_url, headers); // Use GET for placing orders
}
