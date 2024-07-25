#include "ui.hpp"
#include <string>
#include <sstream>
#include <iostream>

int strToNum(std::string str) {
    std::stringstream ss(str);
    int num = 0;
    ss >> num;
    return num;
}



void main_t(std::string buf) {
    // 用数字出命令
    int num = strToNum(buf);
    std::cout << "num: " << num << std::endl;
    if (num == 0) {
        register_ui();
    }
    }



    // 字符串用<|  |>隔开

    // /*1*//* jdskd */
