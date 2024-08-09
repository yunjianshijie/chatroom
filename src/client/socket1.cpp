#include "socket.hpp"
#include "group.hpp"
#include <future>

void Socket::user_run() {
    // 在这里创建异步
    // std::future<std::string> future = std::async([this] { return this->receive_message(); }); //
    // // //这里是选择
    std::thread receive_josn_thread(&Socket::receive_json, this);
    printf("用户 %s 登录成功\n", account.name.c_str());
    while (true) {
        std::string choice = user_main(account.name, account.id);
        this->state = SUCCESS;
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
            showSelfInfo(1);
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
        } else if (choice == "8") { // 添加群聊
            addGroup();
        } else if (choice == "9") { // 创建群聊
            createGroup();
        } else if (choice == "10") { // 查看聊天记录
            showChatRecord();
        } else if (choice == "11") { // 注销账号
            if (cancel()) {
                return;
            }
        } else if (choice == "12") {
            send_json_usr_exit(); // 退出登录
                                  // std::cout << "nihao" << std::endl;

            receive_josn_thread.join();
            break;
        }
    }
}

void Socket::showSelfInfo(int n) // 查看自己信息1
{
    std::cout << "    账号信息 " << std::endl;
    std::cout << "---------------------------------" << std::endl;
    std::cout << " 用户名 : " << account.name << std::endl;
    std::cout << " 用户id : " << account.id << std::endl;
    std::cout << " 用户状态: " << "在线" << std::endl;
    std::cout << " 用户类型: " << "普通用户" << std::endl;
    std::cout << " 用户fd : " << this->client_fd << std::endl;
    std::cout << std::endl;
    std::cout << "---------------------------------" << std::endl;
    if (n == 2) {
        return;
    }
    std::cout << "是否姓名(y/N)" << std::endl;
    std::string choice;
    std::cin >> choice;
    if (choice == "y" || choice == "Y") {
        std::cout << "请输入新的姓名" << std::endl;
        std::string new_name;
        std::cin >> new_name;
        send_change_name(new_name); // 发送更新信息
    }
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
    // 直接发请求看
    send_json_friend_list();
    // 等待结果完成
    std::cout << "选择你要操作的id" << std::endl;
    std::string id;
    std::cin >> id;
    nlohmann::json json;
    int h = 0;
    std::string f_name;
    std::string f_fd;
    for (auto it : this->message_vec) {
        std::cout << it << std::endl;
        json = nlohmann::json::parse(it);
        if (json["id"] == id) {
            h = 1;
            f_name = json["name"];
            f_fd = json["fd"];
            break;
        }
    }
    if (h == 0) {
        std::cout << "没有该好友" << std::endl;
        return;
    }
    std::cout << "选择你要进行的操作" << std::endl;
    std::cout << " <1>  进入聊天页面 " << std::endl;
    std::cout << " <2>  删除好友 " << std::endl;
    std::cout << " <3>  屏蔽好友" << std::endl;
    std::cout << " <4>  取消屏蔽好友" << std::endl;
    std::cout << " <5>  退出" << std::endl;
    std::string choice;
    std::cin >> choice;
    if (choice == "1") {
        // 私聊
        this->private_chat(this->account.id, id, f_name, f_fd);
    } else if (choice == "2") {
        // 删除好友
        nlohmann::json j;
        j["mode"] = FRIEND_DELETE;
        j["id"] = account.id;
        j["friend_id"] = id;
        send_string(j.dump()); // 发送删除好友请求

    } else if (choice == "3") {
        send_json_friend_shield(id);
        //  屏蔽好友
    } else if (choice == "4") {
        // 取消屏蔽好友
        send_json_friend_unshield(id);

    } else {
        return;
    }
    // 取消屏蔽好友
    // 处理结果
}
void Socket::showGroupList() // 查看群聊列表4
{
    nlohmann::json json;
    json["mode"] = GROUP_LIST;
    json["id"] = account.id;
    send_string(json.dump());
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return this->result_ready; });
        this->result_ready = false;
    } // 处理结果
    std::cout << "选择你要操作的序号" << std::endl;
    std::string choice;
    std::cin >> choice;
    int h = 0;
    nlohmann::json j = nlohmann::json::parse(this->buf);
    std::vector<std::string> ret = j["list"];
    std::string gruop = ret[std::stoi(choice) - 1];
    std::cout <<"<1>  进入群聊页面 " << std::endl;
    std::cout <<"<2>  进入管理" << std::endl;
    std::cout <<"<3>  退出" << std::endl;
    std::cin >> choice;
    //这里    
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
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return this->result_ready; });
        this->result_ready = false;
    } // 处理结果
}
void Socket::addGroup() // 添加群聊6
{
}
void Socket::createGroup() // 创建群聊7
{
    std::cout << "创建群聊" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "请输入群聊名称" << std::endl;
    std::string name;
    std::cin >> name;
    nlohmann::json json;
    // 创建群聊
    // 发送，然后给一个id ，群聊id
    // 随机生成
    // 在群聊库里面查有没有这个id
    // 没有服务器发送，有再循环
    // 用共有变量将id返回，不用打印出来应该就行
    // 不用确认
    // 创建群聊
    json["mode"] = GROUP_CREATE;
    json["id"] = account.id;
    json["name"] = name;
    send_string(json.dump()); // 发送创建群聊请求
    // 建立群聊
}
void Socket::showChatRecord() // 查看聊天记录8
{
}
bool Socket::cancel() // 注销账号9
{
    showSelfInfo(2);
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

void Socket::send_change_name(std::string name) {
    this->account.name = name; // 修改用户名
    nlohmann::json json;
    json["mode"] = CHANGE_NAME;
    json["name"] = name;
    json["id"] = account.id;
    account.name = name;
    send_string(json.dump()); // 发送修改信息
}
// 接受函数
std::string Socket::receive_message() {
    try {
        std::cout << "开始接受" << std::endl;
        // char buffer[1024];
        std::string buffer;

        int len = recvMsg(this->server_fd, buffer);
        if (len == -1) {
            throw std::runtime_error("Error receiving data");
        } else if (len == 0) {
            throw std::runtime_error("Connection closed by server");
        }
        printf("接受成功\n");
        std::string momo = buffer;
        return momo; // 返回接收到的字符串
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return ""; // 返回空字符串表示错误
    }
}

void Socket::receive_json() {
    while (true) {
        // printf("dsklf   nhljksdjhflkdsjkf\n");

        // std::string message = receive_message(); // 接收消息
        std::string message;
        int len = recvMsg(this->server_fd, message);
        // std::cout << "收到消息" << message << std::endl;
        if (len == -1) {
            std::cout << "接受失败" << std::endl;
            return;
        } else if (len == 0) {
            std::cout << "服务器断开连接" << std::endl;
            return;
        }
        if (!message.empty() && message != "error" && message != "") {
            int ret = this->receive_josn_user(message); // 处理json
            if (ret == 1) {
                return;
            }
        }
    }
}
// 处理函数 将其放入队列或者直接打印
int Socket::receive_josn_user(std::string message) {
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
    } else if (mode == FRIEND_LIST) {
        this->print_friend_list(message);
    } else if (mode == FRIEND_MESSAGE) {
        this->print_friend_message(message);
    } else if (mode == HISTORY_MESSAGE) {
        this->print_friend_history_message(message);
    } else if (mode == LOGIN_SUCCESS) {
        this->print_login_success(message);
    } else if (mode == FRIEND_SHIELD) {
        this->print_friend_shield(message);
    } else if (mode == FRIEND_UNSHIELD) {
        this->print_friend_unshield(message);
    } else if (mode == FRIEND_SHIELDED) {
        this->print_friend_shielded(message);
    } else if (mode == EXIT) {
        return 1;
    } else if (mode == FRIEND_DLEED) {
        this->print_friend_dleed(message);
    } else if (mode == GROUP_CREATE) {
        this->print_group_create(message);
    } else if (mode == GROUP_LIST) {
        this->print_group_list(message);
    }
    return 0;
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
    {
        std::unique_lock<std::mutex> lock(mtx);
        this->result_ready = true;
        cv.notify_one();
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
    try {
        nlohmann::json json = nlohmann::json::parse(message);
        nlohmann::json array = json["apply_list"];
        std::vector<std::string> vec;
        for (const auto &item : array) {
            // std::cout << "申请人id: " << item["id"] << " 申请人名字: " << item["name"] << std::endl;
            // std::cout << item << std::endl; // 这里处理
            vec.push_back(item);
        }
        this->print_friend_apply_list2(vec);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}
void Socket::print_friend_apply_list2(std::vector<std::string> vec) {
    try {
        int i = 1;
        for (const auto &item : vec) {
            nlohmann::json json = nlohmann::json::parse(item);
            std::string id = json["id"];
            std::string name = json["name"];

            std::cout << "(" << i++ << ")" << "申请人id: " << id << " 申请人名字: " << name << std::endl;
            std::cout << "申请消息：" << json["msg"] << std::endl; // 这里处理
            // std::cout << "申请人id: " << item["id"] << " 申请人名字: " << item["name"] << std::endl;
            //   std::cout << item << std::endl; // 这里处理
        }
        // 这里加锁
        if (vec.empty()) {
            std::cout << "你还没有申请哦～" << std::endl;
            {
                std::unique_lock<std::mutex> lock(mtx);
                this->result_ready = true;
                cv.notify_one();
            }
            return;
        }
        std::cout << "请输入要处理的序号" << std::endl;
        std::string choice;
        std::cin >> choice;
        // 这里处理

        if (std::stoi(choice) >= 1 && std::stoi(choice) <= vec.size()) {
            std::string str = vec[std::stoi(choice) - 1];
            nlohmann::json json = nlohmann::json::parse(str);
            std::string id = json["id"];
            // std::string name = vec[std::stoi(choice) - 1];
            std::cout << "请输入要处理的方式 1.同意 2.拒绝" << std::endl;
            std::string choice2;
            std::cin >> choice2;
            if (choice2 == "1") {
                std::cout << "添加好友成功" << std::endl;
                // 从数据库里面删掉 发东西
                // 且加入好友数据库
            } else if (choice2 == "2") {
                // 从数据库里面删掉
                std::cout << "拒绝添加好友" << std::endl;
            } else {
                std::cout << "输入错误" << std::endl;
                return;
            }
            send_json_friend_apply(this->account.id, id, choice2);
        } else {
            std::cout << "输入错误" << std::endl;
        }
        // 这里解锁
        {
            std::unique_lock<std::mutex> lock(mtx);
            this->result_ready = true;
            cv.notify_one();
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}

std::string Socket::send_json_friend_apply(std::string id, std::string friend_id, std::string chioce) {
    try {
        nlohmann::json json;
        json["mode"] = FRIEND_APPLY;
        json["id"] = id;
        json["friend_id"] = friend_id;
        json["chioce"] = chioce;
        this->send_string(json.dump());
        return json.dump();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    return "";
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
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return this->result_ready; });
            this->result_ready = false;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}
//
std::string Socket::send_json_friend_list() {
    try {
        nlohmann::json json;
        json["mode"] = FRIEND_LIST;
        json["id"] = this->account.id;
        send_string(json.dump());
        return json.dump();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    return "";
}
void Socket::apply_GroupList() {
    try {
        nlohmann::json json;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}

void Socket::print_friend_list(std ::string message) {
    try {
        nlohmann::json json = nlohmann::json::parse(message);
        std::vector<std::string> vec = json["friend_list"];
        this->message_vec = vec;
        this->print_friend_list2(vec);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}
void Socket::print_friend_list2(std::vector<std::string> vec) {
    std::cout << "好友列表" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    try {
        int i = 1;
        for (const auto &item : vec) {
            nlohmann::json json = nlohmann::json::parse(item);
            std::string id = json["id"];
            std::string name = json["name"];
            std::cout << "(" << i++ << ")" << "好友id: " << id << " 好友名字: " << name;
            if (json["fd"] == "-1") {
                std::cout << "   (离线)" << std::endl;
            } else {
                std::cout << "   (在线)" << std::endl;
            }
        }
        std::cout << "---------------------------------------------" << std::endl;
        {
            std::unique_lock<std::mutex> lock(mtx);
            this->result_ready = true;
            cv.notify_one();
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}

void Socket::print_friend_history_message(std::string message) {
    try {
        // std::cout << "print_friend_history_message" << std::endl;
        nlohmann::json json = nlohmann::json::parse(message);

        std::vector<std::string> vec = json["history"];
        if (vec.empty()) {
            std::cout << "无历史消息   开始聊天吧" << std::endl;
            std::cout << "---------------------------------------------" << std::endl;
            return;
        }
        this->print_friend_history_message2(vec);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    {
        std::unique_lock<std::mutex> lock(mtx);
        this->result_ready = true;
        cv.notify_one();
    }
    // 开锁
}
void Socket::print_friend_history_message2(std::vector<std::string> vec) {
    try {
        std::cout << "                   历史消息                  " << std::endl;
        std::cout << "---------------------------------------------" << std::endl;
        int i = 1;
        for (const auto &item : vec) {
            nlohmann::json json = nlohmann::json::parse(item);
            std::string f_id = json["friend_id"];
            // std::string name = json["name"];
            std::string message = json["msg"];
            std::string time = json["time"];
            std::cout << "\033[1;34m(" << i++ << ")" << time << "  id" << f_id << " : " << message << "\033[0m" << std::endl;
        }

        std::cout << "------------以上" << i - 1 << "条历史消息---------------" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}

void Socket::print_login_success(std::string message) {
    try {
        nlohmann::json json = nlohmann::json::parse(message);
        std::vector<std::string> vec = json["message"];
        for (const auto &item : vec) {
            // std::cout << "id" << item << "向你发来消息" << std::endl;
            std::cout << "\033[1;34m你有一条来自id" << item << "消息\033[0m" << std::endl;
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}
void Socket::print_friend_shield(std::string message) {
    try {
        // std::cout << message << std::endl;
        nlohmann::json json = nlohmann::json::parse(message);
        std::string yy = json["y_n"];
        if (yy == "yes") {
            std::cout << "屏蔽成功" << std::endl;
        } else {
            std::cout << "屏蔽失败,你已经屏蔽了" << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}
void Socket::print_friend_unshield(std::string message) {
    try {
        nlohmann::json json = nlohmann::json::parse(message);
        std::string yy = json["y_n"];
        if (yy == "yes") {
            std::cout << "取消屏蔽成功" << std::endl;
        } else {
            std::cout << "取消屏蔽失败,你未屏蔽" << std::endl;
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}

void Socket::print_friend_shielded(std::string message) {
    try {
        nlohmann::json josn = nlohmann::json::parse(message);
        std::string n = josn["or"];
        if (n == "1")
            std::cout << "好可怜，他屏蔽你了，抱抱你" << std::endl;
        if (n == "2")
            std::cout << "好可惜，你屏蔽了他 别发了" << std::endl;
        if (n == "3")
            std::cout << "好悲痛,对方已经不是你的好友了" << std::endl;
        if (n == "4")
            std::cout << "好开心, 你们已经是好友了哦～" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}
void Socket::print_friend_dleed(std::string message) {
    try {
        std::cout << "好伤心，他已经不是你的好友了,去找其他人聊天吧～" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}

void Socket::print_group_create(std::string message) {
    try {
        nlohmann::json json = nlohmann::json::parse(message);
        std::string group_id = json["group_id"];
        std::cout << "创建群聊成功，群聊id为" << group_id << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}

void Socket::print_group_list(std::string message) {
    try {
        nlohmann::json json = nlohmann::json::parse(message);
        std::cout << "群聊列表如下:" << std::endl;
        std::cout << "    群聊id\t群聊名称" << std::endl;
        std::vector<std::string> group_list = json["group_ids"];
        int i = 1;
        for (auto &group : group_list) {
            std::string first_8 = group.substr(0, 8);
            std::string name = group.substr(8, -1);
            std::cout << "\033[1;34m(" << i++ << ")" << first_8 << "\t" << name << "\033[0m" << std::endl;
        }
        std::cout << "请输入要进入的群聊id" << std::endl;
        {
            std::unique_lock<std::mutex> lock(mtx);
            this->buf = message; // 保存群聊列表
            this->result_ready = true;
            cv.notify_one();
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}
