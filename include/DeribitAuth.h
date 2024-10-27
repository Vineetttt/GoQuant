#ifndef DERIBIT_AUTH_H
#define DERIBIT_AUTH_H

#include <string>

class DeribitAuth {
public:
    DeribitAuth(const std::string& clientId, const std::string& clientSecret);
    std::string authenticate();

private:
    std::string clientId;
    std::string clientSecret;
};

#endif
