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

int register_ui() {
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "<1> 登录 " << std::endl;
    std::cout << "<2> 注册 " << std::endl;
    std::cout << "<3> 退出" << std::endl;
    std::cout << "<4> 忘记密码" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "请输入你的选择: " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    int choice;
    std::cin >> choice;
    return choice;
    }

int user_main(std::string UerName, std::string UerIp) {
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "        " << UerName << "  的界面            " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << " <1> 查看自己信息 " << std::endl;
    std::cout << " <2> 查看好友列表 " << std::endl;
    std::cout << " <3> 查看群聊列表" << std::endl;
    std::cout << " <4> 添加好友" << std::endl;
    std::cout << " <5> 添加群聊" << std::endl;
    std::cout << " <6> 创建群聊" << std::endl;
    std::cout << " <7> 查看聊天记录" << std::endl;
    std::cout << " <9> 注销账号" << std::endl;
    std::cout << " <8> 退出登录 " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "请输入你的选择: " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    int choice;
    std::cin >> choice;
    return choice;

}

void user_info(std::string UerName, std::string UerIp) {
}

void login_ui(Account &account) {
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