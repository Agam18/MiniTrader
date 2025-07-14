#include "OrderBook.hpp"
#include <iostream>

void OrderBook::addOrder(const Order& order) {
    std::unique_lock<std::shared_mutex> lock(mtx);
    orders.push_back(order);
}

void OrderBook::printOrders() const {
    std::shared_lock<std::shared_mutex> lock(mtx);
    std::cout << "--- Order Book ---" << std::endl;
    for (const auto& order : orders) {
        std::cout << order.username << ": "
                  << order.action << " "
                  << order.symbol << " "
                  << order.quantity << std::endl;
    }
    std::cout << "------------------" << std::endl;
}
