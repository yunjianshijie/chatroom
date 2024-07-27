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
#include <nlohmann/json.hpp>
#include "ui/ui.hpp"

class Socket {
private:
    /* data */
    int server_fd;
    struct sockaddr_in addr;
    int just;        // 如果是1，则代表非用户操作
                     // 如果是2，则代表用户操作
    std::string buf; // 服务器传回来的信息
    int strToNum(std::string str) {
        std::stringstream ss(str);
        // 如果传回来不是数字就用josn读回来
        int num = 0;
        ss >> num;
        return num;
    }

public:
    int client_fd;
    int mun;
    int mode;
    Socket(int handle) {
        try {
            mode = SUCCESS;
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
            this->buf = buf;
            main_t();
            int s_mode = SUCCESS;
        } catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
        }
    }
    ~Socket() {
        close(client_fd);
    }
    bool send_string(std::string chuan) {
        try {
            ssize_t lee = send(client_fd, chuan.c_str(), chuan.size(), 0);
            if (lee == -1) {
                throw std::runtime_error("Error sending data");
            } else {
                if (lee < chuan.size()) {
                    throw std::runtime_error("Error sending data");
                    // 传大东西这里用多次传那种吧
                }
            }
        } catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
            return false;
        }
        return true;
    }
    std::string receive_string() {
        // 接受数据
        try {
            char buf[1024];
            memset(buf, 0, sizeof(buf));
            int len = recv(client_fd, buf, sizeof(buf), 0);
            if (len == -1) {
                throw std::runtime_error("Error receiving data");
            }
            std::string mm = buf;
            this->buf = mm; // 接受回来的数据
            return mm;
        } catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
            return "";
        }
    }

    std::string main_t() {
        // 处理服务器返回的数据
        // 返回发给服务器的josn数据
        this->mode = strToNum(this->buf);
        std::cout << "mode: " << this->mode << std::endl;
        if (this->mode == SUCCESS) {
            // 第一个页面
            int model = register_ui();
            if (model == LOGIN) { // 登录
                this->just = 2;
                this->buf = "1";
            } else if (model == REGISTER) {
                this->just = 2;
                this->buf = "2";
                // 注册
            }
            nlohmann::json json;
            json["mode"] = model;
            return json.dump();

        } else if (this->mode == FORGET_PASSWORD) {
            // 忘记密码
            // 忘记密码
            // 还没写。。
        }
        return "";
    }
    std::string main_t(Account &account) { // 处理服务器返回的数据
        // 重载
        //  如果是对于账号的操作写这里
        this->mode = strToNum(this->buf);
        std::cout << "mode: " << this->mode << std::endl;
        if (this->mode == LOGIN) { // 登录
            //  Account account;
            // login_ui(account);

        } else if (this->mode == REGISTER) { // 注册
            login_ui(account);
            return account.toJsonString();
        }
        return "";
    }
    std::string get_string() {
        std::string str;
        std::cin >> str;
        return str;
    }
    int r_just(){
        return this->just;
    }
};

// socket::socket(/* args */)
// {
// }

// socket::~socket()
// {
// }
