#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

class Logger {
private:
    std::ofstream logFile;
    static Logger* instance;
    Logger();

public:
    static Logger* getInstance();
    std::string getCurrentTimestamp();
    void log(const std::string& message, const std::string& type = "INFO");
    void logJson(const std::string& prefix, const std::string& jsonStr);
    ~Logger();
};

#endif