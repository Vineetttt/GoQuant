#ifndef GET_ORDER_BOOK_H
#define GET_ORDER_BOOK_H

#include <string>
#include <stdexcept>
#include "HTTPClient.h"
#include <nlohmann/json.hpp>

class GetOrderBook {
public:
    // Constructor
    GetOrderBook();

    // Method to get order book
    std::string getOrderBook(const std::string& instrument_name, int depth = 5);

private:
    std::string buildRequestUrl(const std::string& instrument_name, int depth);
};

#endif
