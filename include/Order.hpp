#pragma once
#include <string>

struct Order {
    std::string username;
    std::string action;  // BUY / SELL
    std::string symbol;
    int quantity;
};
