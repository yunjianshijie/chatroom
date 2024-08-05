#include "ui.hpp"
void begin() {
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "            欢迎来到chatroom              " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
}

void user_long(std::string UerName) {
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "     " << UerName << "   欢迎光临         " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
}

std::string register_ui() {
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "<1> 登录 " << std::endl;
    std::cout << "<2> 注册 " << std::endl;
    std::cout << "<3> 退出" << std::endl;
    std::cout << "<4> 忘记密码" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "请输入你的选择: " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::string choice;
    std::cin >> choice;
    return choice;
}

void user_info(std::string UerName, std::string UerIp) {
}

void register_ui1(Account &account) {
    std::string pass;
    std::string name;
    std::string answer;
    std::string question;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "请输入你的账号名称: " << std::endl;
    std::cin >> name;
    std::cout << "请输入你的密码: " << std::endl;
    std::cin >> pass;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "请输入你的保密问题" << std::endl;
    std::cin >> question;
    std::cout << "请输入你的答案: " << std::endl;
    std::cin >> answer;
    std::cout << "------------------------------------------" << std::endl;
    account.registerAccount(name, pass, question, answer);
    return;
}

int login_ui(Account &account) {
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "输入你的账号id : " << std::endl;
    std::string id;
    std::cin >> id;
    std::cout << "输入你的账号密码: " << std::endl;
    std::string pass;
    std::cin >> pass;
    std::cout << "------------------------------------------" << std::endl;
    account.id = id;
    account.set_pass(pass);
    return 0; // 不用了
    // 给服务器发送登录请求
}

std::string user_change(Account &account) {
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "请输入你的账号id : " << std::endl;
    std::string id;
    std::cin >> id;
    account.id = id;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "你的保密问题： " << std::endl;
    std::cout << account.question << std::endl;
    std::cout << "请输入你的答案: " << std::endl;
    std::string answer;
    std::cin >> answer;
    account.set_answer(answer);
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "请输入你的新密码: " << std::endl;
    std::string pass;
    std::cin >> pass;
    std::cout << "------------------------------------------" << std::endl;
    return pass;
}

std::string user_main(std::string UerName, std::string UerID) {
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "        " << UerName << "  的界面            " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << " <1>  查看个人信息 " << std::endl;
    std::cout << " <2>  查询用户信息 " << std::endl;
    std::cout << " <3>  查看好友列表 " << std::endl;
    std::cout << " <4>  查看群聊列表" << std::endl;
    std::cout << " <5>  添加好友" << std::endl;
    std::cout << " <6>  查看好友申请" << std::endl;
    std::cout << " <7>  查看群聊申请" << std::endl;
    std::cout << " <8>  添加群聊" << std::endl;
    std::cout << " <9>  创建群聊" << std::endl;
    std::cout << " <10> 查看聊天记录" << std::endl;
    std::cout << " <11> 注销账号 " << std::endl;
    std::cout << " <12> 退出登录 " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "请输入你的选择: " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::string choice;
    std::cin >> choice;
    return choice;
}

int user_info_ui() {
    std::cout << "------------------------------------------" << std::endl;
    std::cout << " <1> 查看个人信息 " << std::endl;
    std::cout << " <2> 修改个人信息 " << std::endl;
    std::cout << " <3> 返回上一级 " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "请输入你的选择: " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    int choice;
    std::cin >> choice;
    return choice;
}

std::string user_add_friend_ui() {
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "输入你想加好友的uid: " << std::endl;
    std::string uid;
    std::cin >> uid;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << " " << std::endl;
    return uid; // 给服务器发送添加好友请求
}

std::string change_name_ui() {
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "请输入你的新名字: " << std::endl;
    std::string name;
    std::cin >> name;
    std::cout << "------------------------------------------" << std::endl;
    return name;
}

std::string add_friend_ui() {
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "请输入你要添加的好友的uid: " << std::endl;
    std::string uid;
    std::cin >> uid;
    std::cout << "------------------------------------------" << std::endl;
    return uid;
}
std::string user_query_ui() {
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "请输入你要查询的uid: " << std::endl;
    std::string uid;
    std::cin >> uid;
    std::cout << "------------------------------------------" << std::endl;
    return uid;
}
void user_query_ui1(std::string j, std::string uid) {
    std::cout << "------------------------------------------" << std::endl;
    nlohmann::json j1 = nlohmann::json::parse(j);
    std::cout << " 用户名称：" << j1["name"] << std::endl;
    std::cout << " 用户id:" << uid << std::endl;
    if (j1["fd"] == -1) {
        std::cout << " 用户状态:离线" << std::endl;
    } else {
        std::cout << " 用户状态:在线" << std::endl;
    }
}