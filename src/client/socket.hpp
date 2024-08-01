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
    int just;        // 如果是1，则代表非用户操作, 如果是2，则代表用户操作
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
    int mode;           // mode 记录之后的操作
    Socket(int handle); // handle 是服务器端口号
    ~Socket() {
        close(client_fd); // 关闭客户端的ip
    }
    void socket_do();                                  // 处理主要逻辑
    bool send_string(std::string chuan);               // 发送数据
    std::string receive_string();                      // 接受数据
    bool send_json_usr_register(Account &account);     // 发送注册json数据
    std::string receive_json_usr_id();                 // 接收注册json数据
    bool remember_id(std::string id);                  // 记住id
    Account receive_json_usr_login();                  // 接收登录json数据
    std::string send_json_usr_login(Account &account); // 发送登录json数据

    bool error_string(std::string str); // 错误信息

    // std::string main_t() {
    //     // 处理服务器返回的数据
    //     // 返回发给服务器的josn数据
    //     //
    //     this->mode = strToNum(this->buf);
    //     std::cout << "mode: " << this->mode << std::endl;
    //     // 由于没有改 mode
    //     if (this->mode == SUCCESS) { // 0
    //         // 第一个页面
    //         int model = register_ui();
    //         printf("11\n");
    //         if (model == LOGIN) { // 登录
    //             this->just = 2;
    //             // 这里 不能改服务器传回来的消息
    //             //     this->buf = "1";
    //         } else if (model == REGISTER) {
    //             this->just = 2;
    //             //  this->buf = "2";
    //             // 注册
    //         }
    //         nlohmann::json json;
    //         json["mode"] = model;
    //         return json.dump();

    //     } else if (this->mode == FORGET_PASSWORD) {
    //         // 忘记密码
    //         // 忘记密码
    //         // 还没写。。
    //     }
    //     return "exit";
    // }
    //     std::string main_t(Account &account) { // 处理服务器返回的数据
    //         // 重载
    //         //  如果是对于账号的操作写这里
    //         this->mode = strToNum(this->buf);
    //         std::cout << "mode: " << this->mode << std::endl;
    //         if (this->mode == LOGIN) { // 登录
    //             //  Account account;
    //             // login_ui(account);
    //             this->mode = SUCCESS;
    //         } else if (this->mode == REGISTER) { // 注册
    //             register_ui1(account);
    //             this->mode = SUCCESS;
    //             return account.toJsonString();
    //         }
    //         return "";
    //     }
    //     std::string get_string() {
    //         std::string str;
    //         std::cin >> str;
    //         return str;
    //     }
    //     int r_just() {
    //         return this->just;
    //     }
    //     void r_just(bool just) {
    //         this->just = just;
    //     }
};

// socket::socket(/* args */)
// {
// }

// socket::~socket()
// {
// }
