#include "ui/ui.hpp"
#include "socket.hpp"
#include <cstring>
#include <string>
#include "json.hpp"
class Socket;
int main() {
  
    Socket client(HAND);

    // while (1) {
    //     // 发送数据
    //     std::string xinxi;
    //     std::cin >> xinxi;
    //     std::cout << "你输入了:" << xinxi << std::endl;

    //     printf("开始发送.....\n");

    //     std::string c_push;
    //     //靠服务器上次发来的信号，来判断现在在哪里
    //     c_push = get_josn(xinxi, s_mode);
    //       std::cout << "发送的数据: " << c_push << std::endl;
    //     ssize_t lee = send(fd, c_push.c_str(), c_push.size()+1, 0);
    //     if (lee == -1) {
    //         std::cerr << "Failed to send data" << std::endl;
    //         printf("发送失败");
    //         break;
    //         ssize_t lee = send(fd, xinxi.c_str(), sizeof(xinxi.c_str()), 0);
    //     } else if (lee < sizeof(xinxi.c_str())) {
    //         printf("发送少字");
    //     }
    //     // 接收数据
    //     char buf[1024];
    //     memset(buf, 0, sizeof(buf));
    //     int len = read(fd, buf, sizeof(buf));
    //     std::cout << "服务器说:" << buf << std::endl;
    //     std::string c_get = buf;
    //     s_mode = main_t(c_get);

    // }
    // //关闭套接字
    // // close(fd);
    // return 0;
}

// 连接服务器
// 根据操作得到值
// 显示ui
