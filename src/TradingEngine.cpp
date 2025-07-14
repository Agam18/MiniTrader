#include "TradingEngine.hpp"
#include <sstream>
#include <iostream>

TradingEngine::TradingEngine() {}

std::string TradingEngine::processCommand(const Trader& trader, const std::string& command) {
    std::istringstream iss(command);
    std::string action, symbol;
    int qty;
    iss >> action >> symbol >> qty;

    if ((action != "BUY" && action != "SELL") || symbol.empty() || qty <= 0)
        return "Invalid command";

    Order order{trader.getUsername(), action, symbol, qty};
    orderBook.addOrder(order);

    std::cout << trader.getUsername() << " placed order: "
              << action << " " << symbol << " " << qty << std::endl;

    return "Order accepted: " + action + " " + symbol + " " + std::to_string(qty);
}

void TradingEngine::printOrderBook() const {
    orderBook.printOrders();
}
