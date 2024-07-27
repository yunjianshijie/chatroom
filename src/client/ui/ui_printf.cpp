#include "ui.hpp"
#include "../json.hpp"

int strToNum(std::string str) {
    std::stringstream ss(str);
    int num = 0;
    ss >> num;
    return num;
}

int main_t(std::string buf) {
    // 返回 num
    // 用数字出命令
    int num = strToNum(buf);
    std::cout << "num: " << num << std::endl;
    if (num == SUCCESS) {
        register_ui();
    } else if (num == LOGIN) {
        //     Account account;
        // login_ui(account);
    } else if (num == REGISTER) {
        Account account;
        login_ui(account);
    }
    return num;
}


