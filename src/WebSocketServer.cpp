#include "WebSocketServer.h"
#include <iostream>

DeribitWebSocketServer::DeribitWebSocketServer() {
    server.set_access_channels(websocketpp::log::alevel::all);
    server.clear_access_channels(websocketpp::log::alevel::frame_payload);

    server.init_asio();

    server.set_open_handler([this](ConnectionHdl hdl) { onOpen(hdl); });
    server.set_close_handler([this](ConnectionHdl hdl) { onClose(hdl); });
    server.set_message_handler([this](ConnectionHdl hdl, WebSocketServer::message_ptr msg) { 
        onMessage(hdl, msg); 
    });
}

void DeribitWebSocketServer::start(uint16_t port) {
    server.set_reuse_addr(true);
    server.listen(port);
    server.start_accept();
    std::cout << "WebSocket server started on port " << port << std::endl;
    server.run();
}

void DeribitWebSocketServer::stop() {
    server.stop_listening();
}

void DeribitWebSocketServer::onOpen(ConnectionHdl hdl) {
    std::lock_guard<std::mutex> lock(mutex);
    connections.insert(hdl);
    std::cout << "New connection established" << std::endl;
}

void DeribitWebSocketServer::onClose(ConnectionHdl hdl) {
    std::lock_guard<std::mutex> lock(mutex);
    connections.erase(hdl);
    
    for (auto& subscription : subscriptions) {
        subscription.second.erase(hdl);
    }
    
    std::cout << "Connection closed" << std::endl;
}

void DeribitWebSocketServer::onMessage(ConnectionHdl hdl, WebSocketServer::message_ptr msg) {
    try {
        json request = json::parse(msg->get_payload());
        
        // Handle subscription requests
        if (request.contains("jsonrpc") && request["method"] == "public/subscribe") {
            auto channels = request["params"]["channels"];
            for (const auto& channel : channels) {
                std::string channel_str = channel.get<std::string>();
                // Extract instrument name from channel string (e.g., "book.BTC-PERPETUAL.100ms")
                size_t first_dot = channel_str.find('.');
                size_t second_dot = channel_str.find('.', first_dot + 1);
                if (first_dot != std::string::npos && second_dot != std::string::npos) {
                    std::string instrument = channel_str.substr(first_dot + 1, second_dot - first_dot - 1);
                    
                    std::lock_guard<std::mutex> lock(mutex);
                    subscriptions[instrument].insert(hdl);
                    
                    // Send subscription confirmation
                    json response = {
                        {"jsonrpc", "2.0"},
                        {"method", "subscription"},
                        {"params", {
                            {"channel", channel_str},
                            {"data", json::object()}
                        }}
                    };
                    
                    server.send(hdl, response.dump(), websocketpp::frame::opcode::text);
                }
            }
        }
        // Handle orderbook updates
        else if (request.contains("params") && request["params"].contains("data")) {
            handleOrderBookUpdate(request);
        }
    } catch (const json::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error processing message: " << e.what() << std::endl;
    }
}

void DeribitWebSocketServer::handleOrderBookUpdate(const json& data) {
    if (!data["params"]["data"].contains("instrument_name")) {
        return;
    }

    std::string instrument = data["params"]["data"]["instrument_name"];
    
    std::lock_guard<std::mutex> lock(mutex);
    auto it = subscriptions.find(instrument);
    if (it != subscriptions.end()) {
        for (const auto& connection : it->second) {
            try {
                server.send(connection, data.dump(), websocketpp::frame::opcode::text);
            } catch (const websocketpp::exception& e) {
                std::cerr << "Error sending orderbook update: " << e.what() << std::endl;
            }
        }
    }
}

void DeribitWebSocketServer::notifyClients(const json& orderbook_data) {
    handleOrderBookUpdate(orderbook_data);
}