cmake_minimum_required(VERSION 3.10)
project(trading_system)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

add_executable(server
    src/orderBook.cpp
    src/server.cpp
    src/TradingEngine.cpp
    src/trader.cpp
)

add_executable(client
    src/client.cpp
)
