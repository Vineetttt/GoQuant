#ifndef DERIBIT_TRADING_H
#define DERIBIT_TRADING_H

#include <string>
#include "Logger.h"

class DeribitTrading {
private:
    std::string client_id;
    std::string client_secret;
    std::string access_token;
    Logger* logger;

public:
    DeribitTrading(const std::string& id, const std::string& secret);
    bool authenticate();
    bool placeOrder(const std::string& instrument, double quantity, 
                   const std::string& order_type, const std::string& label, double price);
    bool cancelOrder(const std::string& order_id);
    bool modifyOrder(int64_t order_id, int new_quantity);
    bool getOrderBook(const std::string& instrument_name, int depth);
    bool getPosition(const std::string& instrument_name);
};

#endif