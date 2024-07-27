// #include "ui/ui.hpp"
#include "socket.hpp"
#include <cstring>
#include <string>
#include "json.hpp"
class Socket;
int main() {
    // 创建套接字，并检验传输
    Socket client(HAND);
    Account my_account;
    while (1) {
        // 发送数据
        std::string xinxi = client.main_t();
        if (client.r_just()){
            xinxi = client.main_t(my_account);
        }
            // 输入还没有
            // 靠服务器上次发来的信号，来判断现在在哪里
            std::cout << "发送的数据: " << xinxi << std::endl;
        if (client.send_string(xinxi)) {
            printf("发送成功\n");
        }
        // 接收数据
        std::string c_get;
        c_get = client.receive_string();
        if (c_get == "") {
            continue;
        }
        std::cout << "服务器说:" << c_get << std::endl;
        //   根据服务器返回的值，处理来 改变client.mode
        // 直接在类里面改就可以来
    }
    // //关闭套接字
    // // close(fd);
    return 0;
}

// 连接服务器
// 根据操作得到值
// 显示ui

// ssize_t lee = send(fd, c_push.c_str(), c_push.size()+1, 0);
// if (lee == -1) {
//     std::cerr << "Failed to send data" << std::endl;
//     printf("发送失败");
//     break;
//     ssize_t lee = send(fd, xinxi.c_str(), sizeof(xinxi.c_str()), 0);
// } else if (lee < sizeof(xinxi.c_str())) {
//     printf("发送少字");
// }

// char buf[1024];
// memset(buf, 0, sizeof(buf));
// int len = read(fd, buf, sizeof(buf));
// std::cout << "服务器说:" << buf << std::endl;
// std::string c_get = buf;