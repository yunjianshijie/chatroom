#include<iostream>
#include"ui.hpp"

void begin(){
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "            欢迎来到chatroom              " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
}


void user_long(std::string UerName){
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "     " << UerName << "   欢迎光临         " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
}

void register_ui(){
    std::cout << "<1> 登录 " << std::endl;
    std::cout << "<2> 注册 " << std::endl;
}






