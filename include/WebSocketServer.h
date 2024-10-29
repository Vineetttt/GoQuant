#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <nlohmann/json.hpp>
#include <set>
#include <map>
#include <string>
#include <mutex>

using WebSocketServer = websocketpp::server<websocketpp::config::asio>;
using ConnectionHdl = websocketpp::connection_hdl;
using json = nlohmann::json;

class DeribitWebSocketServer {
public:
    DeribitWebSocketServer();
    void start(uint16_t port);
    void stop();
    void subscribe(const std::string& instrument);
    void notifyClients(const json& orderbook_data);

private:
    WebSocketServer server;
    std::set<ConnectionHdl, std::owner_less<ConnectionHdl>> connections;
    std::map<std::string, std::set<ConnectionHdl, std::owner_less<ConnectionHdl>>> subscriptions;
    std::mutex mutex;

    void onOpen(ConnectionHdl hdl);
    void onClose(ConnectionHdl hdl);
    void onMessage(ConnectionHdl hdl, WebSocketServer::message_ptr msg);
    void handleOrderBookUpdate(const json& data);
};

#endif