#include "DeribitAuth.h"
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <atomic>
#include <condition_variable>

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

class WebSocketClient {
public:
    WebSocketClient() : received_response(false) {
        // Disable all logging
        client_.set_access_channels(websocketpp::log::alevel::none);
        client_.set_error_channels(websocketpp::log::elevel::none);
        
        client_.init_asio();
        client_.set_tls_init_handler(bind(&WebSocketClient::on_tls_init, this));
        client_.set_message_handler(bind(&WebSocketClient::on_message, this, 
            websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
        client_.set_open_handler(bind(&WebSocketClient::on_open, this, 
            websocketpp::lib::placeholders::_1));
    }

    void connect(const std::string& uri) {
        websocketpp::lib::error_code ec;
        connection_ = client_.get_connection(uri, ec);
        
        if (ec) {
            throw std::runtime_error("Connection Error: " + ec.message());
        }

        client_.connect(connection_);
        thread_ = std::thread([this]() { 
            try {
                client_.run();
            } catch (const std::exception& e) {
                throw std::runtime_error(e.what());
            }
        });
    }

    void send_message(const std::string& message) {
        connection_->send(message);
    }

    void close() {
        if (connection_) {
            connection_->close(websocketpp::close::status::normal, "");
        }
        if (thread_.joinable()) {
            client_.stop();
            thread_.join();
        }
    }

    std::string get_response() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait_for(lock, std::chrono::seconds(5), [this]() { return received_response; });
        return response_;
    }

private:
    context_ptr on_tls_init() {
        context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(
            boost::asio::ssl::context::sslv23);
        try {
            ctx->set_options(
                boost::asio::ssl::context::default_workarounds |
                boost::asio::ssl::context::no_sslv2 |
                boost::asio::ssl::context::no_sslv3 |
                boost::asio::ssl::context::single_dh_use
            );
            ctx->set_verify_mode(boost::asio::ssl::verify_none);
        } catch (const std::exception& e) {
            throw std::runtime_error("TLS Error: " + std::string(e.what()));
        }
        return ctx;
    }

    void on_message(websocketpp::connection_hdl hdl, client::message_ptr msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        response_ = msg->get_payload();
        received_response = true;
        cv_.notify_one();
    }

    void on_open(websocketpp::connection_hdl hdl) {}

    client client_;
    client::connection_ptr connection_;
    std::thread thread_;
    std::string response_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool received_response;
};

DeribitAuth::DeribitAuth(const std::string& clientId, const std::string& clientSecret)
    : clientId(clientId), clientSecret(clientSecret) {}

std::string DeribitAuth::authenticate() {
    WebSocketClient ws;
    ws.connect("wss://test.deribit.com/ws/api/v2");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    nlohmann::json auth_message = {
        {"jsonrpc", "2.0"},
        {"id", 9929},
        {"method", "public/auth"},
        {"params", {
            {"grant_type", "client_credentials"},
            {"client_id", clientId},
            {"client_secret", clientSecret}
        }}
    };

    ws.send_message(auth_message.dump());
    std::string response = ws.get_response();
    ws.close();

    return response;
}