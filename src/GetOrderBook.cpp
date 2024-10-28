#include "GetOrderBook.h"

GetOrderBook::GetOrderBook() {}

std::string GetOrderBook::buildRequestUrl(const std::string& instrument_name, int depth) {
    const std::string url = "https://test.deribit.com/api/v2/public/get_order_book";
    
    // Construct the request URL with query parameters
    std::string request_url = url + "?instrument_name=" + instrument_name;
    if (depth > 0) {
        request_url += "&depth=" + std::to_string(depth);
    }
    
    return request_url;
}

std::string GetOrderBook::getOrderBook(const std::string& instrument_name, int depth) {
    // Validate instrument name
    if (instrument_name.empty()) {
        throw std::invalid_argument("Instrument name cannot be empty.");
    }

    // Build the request URL
    std::string request_url = buildRequestUrl(instrument_name, depth);

    // Set headers
    std::vector<std::string> headers = {
        "Content-Type: application/json" // Only necessary for GET requests in some cases
    };

    HttpClient http_client;
    return http_client.get(request_url, headers);
}
