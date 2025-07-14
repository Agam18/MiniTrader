#pragma once
#include "Order.hpp"
#include <vector>
#include <shared_mutex> // instead of <mutex>

class OrderBook {
public:
    void addOrder(const Order& order);
    void printOrders() const;

private:
    mutable std::shared_mutex mtx;
    std::vector<Order> orders;
};
