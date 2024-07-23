#include <iostream>
#include <arpa/inet.h>
#include "epoll.hpp"
#include "socket.hpp"
#include "redis.hpp"


int main() {
    // 通信
    socket_con a;
     if(a.launch()){
        std::cout << "服务器启动成功" << std::endl;
     };
    int cfd[40];
    int count1=0;
    while(1)
{    cfd[count1]=a.Connect_clientfd();
    if(cfd[count1]!=-1){
    std::cout << "连接成功。 客户端文件描述符为 " <<cfd[count1++] <<" 。"<< std::endl; 

    }else{
        std::cout << "连接失败。"<<std::endl;
    }
    std::string str= a.get_new(cfd[count1]);
    std::cout << str <<std::endl;
    
 }       // 解析
        // 分配线程处理
        // 返回有的值
        return 0;
}