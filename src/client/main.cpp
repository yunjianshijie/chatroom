//  #include "ui/ui.hpp"
#include "socket.hpp"
#include <cstring>
#include <string>
#include "threadpool.hpp"
#include <csignal>
#include <future>
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
    char argv1[30] = "0.0.0.0";
    Socket client(HAND, argv1); // 创建套接字并连接"0.0.0.0"

    // // std::thread receiveThread(&Socket::receive_message, std::ref(client)); //接受消息

    // std::future<std::string> future = std::async(std::bind(client.receive_message(), &client)); // 接受消息
    // std::thread processorThread(&process_messages, std::ref(client)); //这个线程判断是否要给发送线程发送消息
    client.socket_do();

    // receiveThread.join();
    // processorThread.join();

    return 0;
}
