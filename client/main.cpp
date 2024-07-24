#include"ui/ui.hpp"
#include<iostream>
#include"socket.hpp"
#include <stdio.h>
int main() {
    // 1. 创建通信的套接字 
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(0);
    }
 std::cout << "开始连接" << std::endl;
    // 2. 连接服务器
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080); // 大端端口
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr.s_addr);
  
    int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        perror("connect");
        exit(0);
    }  
    begin();
    std:: cout <<"你好" <<std::endl;

    // 3. 和服务器端通信
    int number = 0;
    while (1) {
        // 发送数据
        char buf[1024] ="dddddddddd";
        std::string xinxi;
        std::cin >> xinxi;
        printf("开始发送.....\n");
        
        ssize_t lee =send(fd, buf, sizeof(buf),0);
        if(lee==-1){
            std::cerr << "Failed to send data" << std:: endl;
            printf("发送失败");
            break;
        }else if(lee<sizeof(buf)){
            printf("发送少字");
        }
        // 接收数据
        memset(buf, 0, sizeof(buf));
        int len = read(fd, buf, sizeof(buf));
        std::cout <<"服务器说:" <<buf <<std::endl;

    
    }
  }
    // 连接服务器      
    // 根据操作得到值
    // 显示ui
