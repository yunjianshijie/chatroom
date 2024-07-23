#include "socket.hpp"
// 连接客户端



// int main() {
//     return 0;
// }

// int main() {
//     // int socket(int domain, int type, int protocol);
//     int std = socket(AF_INET, SOCK_STREAM, 0);
//     // 创建套接字
//     if (std == -1) {
//         perror("socket");
//         exit();
//     }
//     // 将socket()返回值和本地的IP端口绑定到一起
//     struct sockaddr_in addr;
//     addr.sin_family = AF_INET;
//     addr.sin_port = htons(8888);
//     addr.sin_addr.s_addr = INADDR_ANY; // 这个宏的值为0 == 0.0.0.0
//     int ret = bind(lfd, (struct sockaddr *)&addr, sizeof(addr));
//     if (ret == -1)
//         ;
//     {
//         perror("bind");
//         exit(0);
//     }
//     while (1) {
//         re
//         // 监听

//         //

//         // 用epoll处理通信并返回
//     }
//     return 0;
// }