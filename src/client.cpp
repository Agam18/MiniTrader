#include <iostream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket error");
        return -1;
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    // Receive prompt from server: "Enter your username:"
    char recvbuf[1024];
    ssize_t n = recv(sock, recvbuf, sizeof(recvbuf)-1, 0);
    if (n > 0) {
        recvbuf[n] = '\0';
        std::cout << recvbuf;
    }

    // Enter username & send
    std::string input;
    std::getline(std::cin, input);
    send(sock, input.c_str(), input.size(), 0);

    // Loop for further commands
    while (true) {
        std::getline(std::cin, input);
        if (input == "EXIT") break;
        send(sock, input.c_str(), input.size(), 0);
        n = recv(sock, recvbuf, sizeof(recvbuf)-1, 0);
        if (n <= 0) break;
        recvbuf[n] = '\0';
        std::cout << recvbuf << std::endl;
    }

    close(sock);
    return 0;
}
