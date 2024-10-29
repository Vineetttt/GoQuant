#include "Logger.h"
#include <chrono>
#include <iomanip>
#include <sstream>

Logger* Logger::instance = nullptr;

Logger::Logger() {
    std::string timestamp = getCurrentTimestamp();
    logFile.open("trading_log_" + timestamp + ".log", std::ios::app);
}

Logger* Logger::getInstance() {
    if (instance == nullptr) {
        instance = new Logger();
    }
    return instance;
}

std::string Logger::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y%m%d_%H%M%S");
    return ss.str();
}

void Logger::log(const std::string& message, const std::string& type) {
    std::string timestamp = getCurrentTimestamp();
    logFile << "[" << timestamp << "] [" << type << "] " << message << std::endl;
    logFile.flush();
}

void Logger::logJson(const std::string& prefix, const std::string& jsonStr) {
    try {
        auto j = nlohmann::json::parse(jsonStr);
        log(prefix + ": " + j.dump(4));
    } catch (const std::exception& e) {
        log(prefix + " (Raw): " + jsonStr);
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}