#include "ui/ui.hpp"
#include <iostream>
#include "socket.hpp"
#include <nlohmann / json.hpp>
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
    addr.sin_port = htons(8081); // 大端端口
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr.s_addr);

    int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        perror("connect");
        exit(0);
    }
    begin();
    std::cout << "你好" << std::endl;

    // 3. 和服务器端通信
    int number = 0;
    std::cout << "检测通信正常..." << std::endl;
    char test1[10] = "<|1|>";

    ssize_t lee = send(fd, test1, sizeof(test1), 0);

    if (lee == -1) {
        std::cerr << "Failed to send data" << std::endl;
        printf("发送失败");
        ssize_t lee = send(fd, test1, sizeof(test1), 0);
        return 1;
    } else if (lee < sizeof(test1)) {
        printf("发送少字");
        return 1;
    }
    std::cout << "发送正常..." << std::endl;
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    int len = read(fd, buf, sizeof(buf));
    if (len == -1) {
        printf("接受失败");
        return 1;
    }
    std::string mm = buf;
    main_t(mm);

    while (1) {
        // 发送数据
        std::string xinxi;
        std::cin >> xinxi;
        std::cout << "你输入了:" << xinxi << std::endl;

        printf("开始发送.....\n");
        ssize_t lee = send(fd, xinxi.c_str(), sizeof(xinxi.c_str()), 0);
        if (lee == -1) {
            std::cerr << "Failed to send data" << std::endl;
            printf("发送失败");
            break;
            ssize_t lee = send(fd, xinxi.c_str(), sizeof(xinxi.c_str()), 0);
        } else if (lee < sizeof(xinxi.c_str())) {
            printf("发送少字");
        }
        // 接收数据
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        int len = read(fd, buf, sizeof(buf));
        std::cout << "服务器说:" << buf << std::endl;
        std::string c_get = buf;
        main_t(c_get);
    }
    //关闭套接字
    close(fd);
    return 0;

}

// 连接服务器
// 根据操作得到值
// 显示ui
