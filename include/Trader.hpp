#pragma once
#include <string>

class Trader {
public:
    Trader(const std::string& name);
    std::string getUsername() const;

private:
    std::string username;
};
