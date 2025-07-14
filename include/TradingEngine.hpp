#pragma once
#include "OrderBook.hpp"
#include "Trader.hpp"
#include <string>

class TradingEngine {
public:
    TradingEngine();

    std::string processCommand(const Trader& trader, const std::string& command);
    void printOrderBook() const;

private:
    OrderBook orderBook;
};
