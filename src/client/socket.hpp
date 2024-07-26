#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <optional>
#include <stdlib.h>
#include <unistd.h>
#include "../include/back.hpp"
#include "ui/ui.hpp"

class Socket {
private:
    /* data */
    int client_fd;
    int server_fd;
    struct sockaddr_in addr;
    int mun;

public:
    Socket(int handle) {
        try {
            client_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (client_fd < 0) {
                throw std::runtime_error("Error creating socket");
            }
            addr.sin_family = AF_INET;
            addr.sin_port = htons(HAND); // 大端端口
            inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr.s_addr);

            int ret = connect(client_fd, (struct sockaddr *)&addr, sizeof(addr));
            if (ret == -1) {
                throw std::runtime_error("Error connecting to server");
            }
            begin();
            // std::cout << "你好" << std::endl;
            int number = 0;
            std::cout << "检测通信正常..." << std::endl;
            char test1[100] = "{\"mode\": 0}";
            // 发送0
            ssize_t lee = send(client_fd, test1, sizeof(test1), 0);

            if (lee == -1) {
                std::cerr << "Failed to send data" << std::endl;
                printf("发送失败");
                ssize_t lee = send(client_fd, test1, sizeof(test1), 0);
                throw std::runtime_error("Error sending data");
            } else if (lee < sizeof(test1)) {
                printf("发送少字");
                throw std::runtime_error("Error sending data");
            }
            std::cout << "发送正常..." << std::endl;
            char buf[1024];
            memset(buf, 0, sizeof(buf));
            int len = read(client_fd, buf, sizeof(buf));
            if (len == -1) {
                printf("接受失败");
                throw std::runtime_error("Error receiving data");
            }
            std::string mm = buf;
            main_t(mm);
            int s_mode = SUCCESS;
        } catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
        }
    }
    ~Socket() {
        close(client_fd);
    }
    

};

// socket::socket(/* args */)
// {
// }

// socket::~socket()
// {
// }
