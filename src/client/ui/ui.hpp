#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../../include/account.hpp"
#include "../../include/back.hpp"
void begin();
void user_long(std::string UerName);
std::string register_ui(); //开始页面
void register_ui1(Account &account); //在注册时输入密码
int login_ui(Account &account);
// 处理数字
// int main_t(std::string buf);
// int strToNum(std::string str);
