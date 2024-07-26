#include "chuli.hpp"
#include <vector>
#include <string>
#include <cstring>
#include <nlohmann/json.hpp>
// using json = nlohmann::json;

std::string numToStr(int num) {
    return std::to_string(num);
}

std::string fanhui(char *buffer, int num) {
    // 这里是处理主函数
    // 这里默认buffer为 josn
    std::cout << "buffer:" << buffer << std::endl;
    std::string str(buffer);
    std::cout << "str:" << str << std::endl;
    nlohmann::json j = nlohmann::json::parse(str);
    // 创建类型
    // 解析
  
    int  mode = j["mode"];
    std::cout << "mode:" << mode << std::endl;
    if (mode == 0) {
        // 如果是
        std::string h = numToStr(0);
        return h;
    } else if (mode == LOGIN) { //登录
        std::string h = numToStr(1);
        return h;
    }else if(mode == REGISTER)//注册
    {
        std::string h = numToStr(2);
        return h;
    }
    return " error";
    // return "hello";
}

