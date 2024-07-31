// #include "ui/ui.hpp"
#include "socket.hpp"
#include <cstring>
#include <string>
#include "json.hpp"
#include "threadpool.hpp"
// class Socket;
int main() {
    // 创建套接字，并检验传输
    Socket client(HAND); // 创建套接字并连接
    Account my_account;  // 创建线程账号
    ThreadPool pool(10); // 创建线程池
    while (1) {
        // 发送数据
        // pool.enqueue([&client, &my_account] {
        // });
        std::string xinxi = client.main_t();
        if (client.r_just()) {
            xinxi = client.main_t(my_account);
        }
        // 输入还没有
        // 靠服务器上次发来的信号，来判断现在在哪里
        std::cout << "发送的数据: " << xinxi << std::endl;
        if (client.send_string(xinxi)) {
            printf("发送成功\n");
        }
        // 接收数据
        // pool.enqueue([&client] {

        // });
        std::string c_get;
        c_get = client.receive_string();
        if (c_get == "") {
            // continue;
            return;
        }
        std::cout << "服务器说:" << c_get << std::endl;
        //   根据服务器返回的值，处理来 改变client.mode
        // 直接在类里面改就可以来
    }
    //关闭套接字
    // close(fd);
    return 0;
}