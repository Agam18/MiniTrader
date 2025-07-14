#include <iostream>
#include <thread>
#include <netinet/in.h>
#include <unistd.h>
#include "TradingEngine.hpp"
#include "Trader.hpp"

#define PORT 8080

TradingEngine engine;

void handle_client(int client_socket) {
    char buffer[1024];

    // Step 1: ask username
    std::string ask = "Enter your username: ";
    send(client_socket, ask.c_str(), ask.size(), 0);

    ssize_t bytes_read = recv(client_socket, buffer, sizeof(buffer)-1, 0);
    if (bytes_read <= 0) {
        close(client_socket);
        return;
    }
    buffer[bytes_read] = '\0';
    std::string username(buffer);
    Trader trader(username);
    std::cout << "User connected: " << username << std::endl;

    // Step 2: process commands
    while (true) {
        bytes_read = recv(client_socket, buffer, sizeof(buffer)-1, 0);
        if (bytes_read <= 0) break;

        buffer[bytes_read] = '\0';
        std::string response = engine.processCommand(trader, buffer);
        send(client_socket, response.c_str(), response.size(), 0);
    }
    close(client_socket);
    std::cout << username << " disconnected." << std::endl;
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

sockaddr_in address{};
address.sin_family = AF_INET;
address.sin_port = htons(PORT);
address.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    // ⭐ Start admin thread for server commands
    std::thread([](){
        std::string admin_cmd;
        while (true) {
            std::getline(std::cin, admin_cmd);
            if (admin_cmd == "print orderbook") {
                engine.printOrderBook();
            } else if (admin_cmd == "exit") {
                std::cout << "Server exit command received. Exiting...\n";
                exit(0);
            } else if (!admin_cmd.empty()) {
                std::cout << "Unknown admin command: " << admin_cmd << std::endl;
            }
        }
    }).detach();

    // ⭐ Accept clients as before
    while (true) {
        int client_socket = accept(server_fd, nullptr, nullptr);
        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }
        std::thread(handle_client, client_socket).detach();
    }

    return 0;
}
