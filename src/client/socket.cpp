#include "socket.hpp"
Socket::Socket(int handle) {
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
        // main_t();
        int s_mode = SUCCESS;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}

void Socket::socket_do() {
    // std::string c_get;
    // c_get = this->receive_string();
    // std::cout << "Received: " << c_get << std::endl;
    begin();
    Account account; // 本机账号信息
    try {
        do {
            std::string choice = register_ui();
            if (choice == "1") { // 登录
                login_ui(account);
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                // 清理缓存区
                // 从redis里面找到账号信息

            } else if (choice == "2") { // 注册
                Account account1;
                register_ui1(account1); //
                this->send_json_usr(account1); // 发送json
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清理缓存区
                //要接收服务器返回的id
                std::string id = this->receive_json_usr_id();
                if(!id.empty()){ //如果id不为空
                
                }
                std::cout << "注册成功" << std::endl;
            } else if (choice == "3") {
                exit(0);
            } else {
                std::cout << "输入错误，请重新输入" << std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        } while (true);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}

bool Socket::remember_id(std::string id){ //是否记住id了
    std::string yes ="";
    do {
        std::cout << "你的id是:" << id << std::endl;
        std::cout << "是否记住id(y/n):"<< std::endl;
        std::cin >> yes;
    } while (yes != "y");
    return true;
}

std::string Socket::receive_json_usr_id(){// 接收注册json数据
    try
    {
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        int len = recv(client_fd, buf, sizeof(buf), 0);
        if (len == -1)
        {
            throw std::runtime_error("Error receiving data");
        }
        std::string mm = buf;
        this->buf = mm;
        return mm; // 接受回来的数据
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return "";
    }
} 

bool Socket::send_string(std::string chuan) {
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
std::string Socket::receive_string() {
    // 接受数据
    printf("zzzz");
    try {
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        int len = recv(client_fd, buf, sizeof(buf), 0);
        if (len == -1) {
            throw std::runtime_error("Error receiving data");
        }
        std::cout << "接受正常..." << std::endl;
        std::string mm = buf;
        this->buf = mm;
        return mm; // 接受回来的数据
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return "exit";
    }
}
bool Socket::send_json_usr(Account &account) {
    try {
        std::string json_str;
        nlohmann::json j;
        j["mode"] = REGISTER; // 注册模式
        j["name"] = account.name;
        j["password"] = account.get_password();
        j["question"] = account.question;
        j["answer"] = account.get_answer();
        // std::string json_str = account.to_json();
        // return send_string(json_str);
        // 直接发了
        json_str = j.dump();
        return send_string(json_str);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}