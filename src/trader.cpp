#include "Trader.hpp"

Trader::Trader(const std::string& name) : username(name) {}

std::string Trader::getUsername() const {
    return username;
}
