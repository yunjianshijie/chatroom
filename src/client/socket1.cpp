#include "socket.hpp"
#include <future>

void Socket::user_run() {
    // 在这里创建异步
    // std::future<std::string> future = std::async([this] { return this->receive_message(); }); //
    // // //这里是选择
    std::thread receive_josn_thread(&Socket::receive_json, this);
    printf("用户 %s 登录成功\n", account.name.c_str());
    while (true) {
        std::string choice = user_main(account.name, account.id);
        // //这里是选择
        // std::cout << " <1>  查看个人信息 " << std::endl;
        // std::cout << " <2>  查询用户信息 " << std::endl;
        // std::cout << " <3>  查看好友列表 " << std::endl;
        // std::cout << " <4>  查看群聊列表" << std::endl;
        // std::cout << " <5>  添加好友" << std::endl;
        // std::cout << " <6>  查看好友申请" << std::endl;
        // std::cout << " <7>  查看群聊申请" << std::endl;
        // std::cout << " <8>  添加群聊" << std::endl;
        // std::cout << " <9>  创建群聊" << std::endl;
        // std::cout << " <10> 查看聊天记录" << std::endl;
        // std::cout << " <11> 注销账号 " << std::endl;
        // std::cout << " <12> 退出登录 " << std::endl;
        if (choice == "1") {
            showSelfInfo();
        } else if (choice == "2") { // 查询用户信息
            queryUser();
        } else if (choice == "3") { // 查看好友列表
            showFriendList();
        } else if (choice == "4") { // 查看群聊列表
            showGroupList();
        } else if (choice == "5") { // 添加好友
            addFriend();
        } else if (choice == "6") { // 查看好友申请
            apply_FriendList();
        } else if (choice == "7") { // 查看群聊申请
            apply_GroupList();
        } else if (choice == "8") {
            addGroup();
        } else if (choice == "9") { // 创建群聊
            createGroup();
        } else if (choice == "10") {
            showChatRecord();
        } else if (choice == "11") { // 注销账号
            if (cancel()) {
                return;
            }
        } else if (choice == "12") {
            send_json_usr_exit(); // 退出登录
            return;
        }
    }
}

void Socket::showSelfInfo() // 查看自己信息1
{
    std::cout << "    账号信息 " << std::endl;
    std::cout << "---------------------------------" << std::endl;
    std::cout << " 用户名 : " << account.name << std::endl;
    std::cout << " 用户id : " << account.id << std::endl;
    std::cout << " 用户状态: " << "在线" << std::endl;
    std::cout << " 用户类型: " << "普通用户" << std::endl;
    std::cout << " 用户fd : " << this->client_fd << std::endl;
    std::cout << std::endl;
}

void Socket::queryUser() { // 查询用户2
    std::string id = user_query_ui();
    send_json_user_que(id);
    // 发送查询信息
    // 等待结果完成
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return this->result_ready; });
        this->result_ready = false;
    } // 处理结果
}

void Socket::showFriendList() // 查看好友列表3
{
}
void Socket::showGroupList() // 查看群聊列表4
{
}
void Socket::addFriend() // 添加好友5
{
    std::string id = add_friend_ui();
    if (id == account.id) {
        std::cout << "不能添加自己为好友" << std::endl;
        return;
    }
    std::string msg;
    std::cout << "请输入添加好友的消息" << std::endl;
    std::cin >> msg;
    nlohmann::json json;
    json["mode"] = FRIEND_ADD;
    json["id"] = account.id;
    json["msg"] = msg;
    json["friend_id"] = id;
    json["name"] = account.name;
    send_string(json.dump()); // 发送添加好友请求
    // 查询用户

    // {
    //     std::unique_lock<std::mutex> lock(mtx);
    //     cv.wait(lock, [this] { return this->result_ready; });
    //     this->result_ready = false;
    // } // 处理结果
}
void Socket::addGroup() // 添加群聊6
{
}
void Socket::createGroup() // 创建群聊7
{
}
void Socket::showChatRecord() // 查看聊天记录8
{
}
bool Socket::cancel() // 注销账号9
{
    showSelfInfo();
    std::cout << "你确定要注销账号吗(y/N)" << std::endl;
    std::string choice;
    std::cin >> choice;
    if (choice == "y" || choice == "Y") {
        nlohmann::json json;
        json["mode"] = DELETE_SELF1;
        json["id"] = account.id;
        send_string(json.dump());
        return true;
    }
    return false; // 不注销
}

//

void Socket::change_name() {
    std::string name = change_name_ui();
    this->account.name = name; // 修改用户名
    nlohmann::json json;
    json["mode"] = CHANGE_NAME;
    json["name"] = name;
}
// 接受函数
std::string Socket::receive_message() {
    try {
        // std::cout << "开始接受" << std::endl;
        char buffer[1024];
        ssize_t len = recv(this->server_fd, buffer, sizeof(buffer), 0);
        if (len == -1) {
            throw std::runtime_error("Error receiving data");
        } else if (len == 0) {
            throw std::runtime_error("Connection closed by server");
        }
        printf("接受成功\n");
        // std::cout << "接受长度" << len << std::endl;
        // std::cout << "接受内容" << buffer << std::endl;
        std::string momo = buffer;
        return momo; // 返回接收到的字符串
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return ""; // 返回空字符串表示错误
    }
}

void Socket::receive_json() {
    while (true) {
        std::string message = receive_message();
        // if (message.empty()) {
        //     std::cout << "Connection closed by server" << std::endl;
        //     break;
        //     }
        std::cout << "收到消息" << message << std::endl;
        if (!message.empty() && message != "error" && message != "") {
            this->receive_josn_user(message); // 处理json
        }
    }
}
// 处理函数 将其放入队列或者直接打印
void Socket::receive_josn_user(std::string message) {
    nlohmann::json json = nlohmann::json::parse(message);
    int mode = json["mode"];
    if (mode == USER_QUERY) {
        this->print_user_qu(message);
    } else if (mode == FRIEND_ADD) {
        this->print_friend_add(message);
    } else if (mode == FRIEND_ADD_2) {
        this->print_friend_add_2(message);
    } else if (mode == FRIEND_APPLY_LIST) {
        this->print_friend_apply_list(message);
    }
}
//
void Socket::print_friend_add(std::string message) {
    nlohmann::json json = nlohmann::json::parse(message);
    if (json["fd"] == "-2") {
        std::cout << "用户不存在" << std::endl;
    } else if (json["fd"] == "-1") {
        std::cout << "用户不在线" << std::endl;
        std::cout << "在线了给ta说哦~" << std::endl;
    } else {
        std::cout << "申请已发送" << std::endl;
    }
}

void Socket::print_friend_add_2(std::string message) {
    nlohmann::json json = nlohmann::json::parse(message);
    std::string id = json["id"];
    std::string name = json["name"];
    std::cout << "\033[35m    你有一个好友申请   \033[37m" << std::endl;
    std::cout << "申请人id为" << id << "名字为" << name << std::endl;
}

void Socket::print_user_qu(std::string message) {
    nlohmann::json json = nlohmann::json::parse(message);
    std::string name = json["name"];
    std::string id = json["id"];
    std::string fd = json["fd"];
    if (fd == "-2") {
        std::cout << "用户不存在" << std::endl;
    } else {
        std::cout << "---------------------------------" << std::endl;
        std::cout << "用户名: " << name << std::endl;
        std::cout << "用户id: " << id << std::endl;
        if (fd == "-1") {
            std::cout << "用户状态: 离线" << std::endl;
        } else {
            std::cout << "用户状态: 在线" << std::endl;
        }
        std::cout << "---------------------------------" << std::endl;
    }
    // 这里解锁
    {
        std::unique_lock<std::mutex> lock(mtx);
        this->result_ready = true;
        cv.notify_one();
    }
    // std::mutex mtx;
    // std::condition_variable cv;
    // bool thread_1_finished = false;
}

void Socket::print_friend_apply_list(std::string message) {
    //是因为json把\" 转义成了" 所以要转义回来
    try{
    nlohmann::json json = nlohmann::json::parse(message);
    std::string frr = json["apply_list"];
    nlohmann::json friends = nlohmann::json::parse(frr);
    int i = 1;
    std::cout << "好友申请列表" << std::endl;
     for (const auto &friend_json : friends) {
         std::string id = friend_json["id"];
         std::string name = friend_json["name"];
         std::cout << "(" << i++ << ")" << " 申请人id: " << id << " 申请人名字: " << name << std::endl;
     }
    std::cout << "请输入要处理的好友序号" << std::endl;
    std::string choice;
    std::cin >> choice;
    //这里处理
    if (std::stoi(choice) >= 1 && std::stoi(choice) <= friends.size()) {
        std::string id = friends[std::stoi(choice) - 1]["id"];
        std::string name = friends[std::stoi(choice) - 1]["name"];
        std::cout << "请输入要处理的方式 1.同意 2.拒绝" << std::endl;
        std::string choice2;
        std::cin >> choice2;
        if (choice2 == "1") {
            std::cout << "添加好友成功" << std::endl;
            // 从数据库里面删掉
            // 且加入好友数据库
        } else {
            std::cout << "拒绝添加好友" << std::endl;
            // 从数据库里面删掉
        }
    } else {
        std::cout << "输入错误" << std::endl;
    }}catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}

// 发送询问 send_json_user_que
std::string Socket::send_json_user_que(std::string id) {
    try {
        nlohmann::json json;
        json["mode"] = USER_QUERY;
        json["id"] = id;
        send_string(json.dump());
        return json.dump();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return "";
    }
    return "";
}
// //好友申请列表6
void Socket::apply_FriendList() {
    try {
        nlohmann::json json;
        json["mode"] = FRIEND_APPLY_LIST;
        json["id"] = this->account.id;
        send_string(json.dump());
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}
//
void Socket::apply_GroupList() {
    try {
        nlohmann::json json;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}
// std::string Socket::send_string(std::string chuan) {
//     try {
//         ssize_t lee = send(this->fd, chuan.c_str(), chuan.size(), 0);
//         if (lee == -1) {
//             throw std::runtime_error("Error sending data");
//         } else if (lee < chuan.size()) {
//             throw std::runtime_error("Error sending data");
//             // 传大东西这里用多次传那种吧
//         } else {
//             return chuan; // 成功
//         }
//     } catch (const std::exception &e) {
//         std::cerr << e.what() << '\n';
//         return "";
//     }
//     return "";
// }
