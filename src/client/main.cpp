//  #include "ui/ui.hpp"
#include "socket.hpp"
#include <cstring>
#include <string>
#include <csignal>
#include <future>
#include "group.hpp"
// class Socket;

// void sig_handler(int signum) {
//     if (signum == SIGINT) { // Ctrl+C
//         std::cout << "Received Ctrl+C signal, exiting program." << std::endl;
//         exit(0);
//     } else if (signum == SIGTERM) { // Ctrl+D
//         std::cout << "Received Ctrl+D signal, exiting program." << std::endl;
//         exit(0);
//     }
// }

int main(int argc, char **argv) {
    // 信号处理
    // std::signal(SIGINT, sig_handler);
    // std::signal(SIGTERM, sig_handler);
    // 创建套接字，并检验传输
    int hand = HAND;
    if (argc == 2) {
        std::string top = argv[1];
        hand = std::stoi(top);
    }
    
    char argv1[30] = "0.0.0.0";
    Socket client(hand, argv1); // 创建套接字并连接"0.0.0.0"
    client.socket_do();         // 进行传输
    return 0;
}
