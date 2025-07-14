#include <iostream>
#include <string>
#include <map>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <thread>
#include "TradingEngine.hpp"
#include "Trader.hpp"

#define PORT 8080
#define MAX_EVENTS 1024

TradingEngine engine;
std::map<int, Trader> clients; // map: client_fd → Trader

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        return 1;
    }

    if (listen(server_fd, 128) < 0) {
        perror("listen failed");
        return 1;
    }

    set_nonblocking(server_fd);

    int kq = kqueue();
    if (kq == -1) {
        perror("kqueue failed");
        return 1;
    }

    struct kevent ev_set;
    EV_SET(&ev_set, server_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(kq, &ev_set, 1, NULL, 0, NULL);

    std::cout << "Server listening on port " << PORT << "...\n";

    // ⭐ Start admin thread
    std::thread([](){
        std::string admin_cmd;
        while (true) {
            std::getline(std::cin, admin_cmd);
            if (admin_cmd == "print orderbook") {
                engine.printOrderBook();
            } else if (admin_cmd == "exit") {
                std::cout << "Server exiting...\n";
                exit(0);
            } else if (!admin_cmd.empty()) {
                std::cout << "Unknown admin command: " << admin_cmd << std::endl;
            }
        }
    }).detach();

    struct kevent events[MAX_EVENTS];

    while (true) {
        int nev = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
        if (nev < 0) {
            perror("kevent error");
            continue;
        }

        for (int i = 0; i < nev; ++i) {
            if (events[i].flags & EV_ERROR) {
                std::cerr << "EV_ERROR: " << strerror((int)events[i].data) << std::endl;
                continue;
            }

            if (events[i].ident == server_fd) {
                // Accept new client
                sockaddr_in cli_addr{};
                socklen_t cli_len = sizeof(cli_addr);
                int client_fd = accept(server_fd, (sockaddr*)&cli_addr, &cli_len);
                if (client_fd < 0) continue;

                set_nonblocking(client_fd);
                EV_SET(&ev_set, client_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                kevent(kq, &ev_set, 1, NULL, 0, NULL);

                // Ask for username
                std::string prompt = "Enter your username: ";
                send(client_fd, prompt.c_str(), prompt.size(), 0);
            } else {
                int client_fd = events[i].ident;
                char buf[1024];
                ssize_t n = recv(client_fd, buf, sizeof(buf)-1, 0);
                if (n <= 0) {
                    close(client_fd);
                    clients.erase(client_fd);
                    continue;
                }
                buf[n] = '\0';
                std::string msg(buf);

                auto it = clients.find(client_fd);
                if (it == clients.end()) {
                    // First message → treat as username
                    clients.emplace(client_fd, Trader(msg));
                    std::cout << msg << " connected." << std::endl;
                    std::string welcome = "Welcome, " + msg + "!\n";
                    send(client_fd, welcome.c_str(), welcome.size(), 0);
                } else {
                    Trader& trader = it->second;
                    std::string response = engine.processCommand(trader, msg);
                    response += "\n";
                    send(client_fd, response.c_str(), response.size(), 0);
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
