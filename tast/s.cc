// Server.cpp
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

#define BUFFER_SIZE 1024
#define CLIENT_SIZE 1024

int clientSocket[CLIENT_SIZE];
int count = 0;

void recv_func(int idx);

int main() {
    printf("Server:\n");
    // 2. 创建socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == -1) {
        perror("socket");
        exit(1);
    }

    // 3. 创建地址协议族
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1111); // 大端端口
    addr.sin_addr.s_addr = INADDR_ANY;
    // 4. 绑定
    int r = bind(serverSocket, (sockaddr *)&addr, sizeof(addr));
    if (r == -1) {
        perror("bind");
        exit(1);
    }

    // 5. 监听
    r = listen(serverSocket, 10);
    if (r == -1) {
        perror("listen");
        exit(1);
    }

    // 6. 等待客户端连接
    struct sockaddr_in cAddr = {0};
    socklen_t len = sizeof(cAddr);

    while (1) {
        clientSocket[count] = accept(serverSocket, (sockaddr *)&cAddr, &len);
        if (clientSocket[count] == -1) {
            perror("accept");
        }
        printf("client %d ---------> adrress: %s! port: %u16\n", count, inet_ntoa(cAddr.sin_addr), cAddr.sin_port);

        // 7. （创建线程）通信
        std::thread RecvMsg(&recv_func, count);
        RecvMsg.detach();

        count++;
    }

    close(serverSocket);
    return 0;
}

void recv_func(int idx) {
    char recv_buff[BUFFER_SIZE];
    while (1) {
        memset(recv_buff, 0, BUFFER_SIZE);
        int r = recv(clientSocket[idx], recv_buff, sizeof(recv_buff), 0);
        if (r > 0) {
            printf("Clinet %d -----> Server : %s\n", idx, recv_buff);
        }

        // 广播给所有client
        for (int i = 0; i < count; i++) {
            if (i != idx) {
                send(clientSocket[i], recv_buff, sizeof(recv_buff), 0);
            }
        }

        // 输入quit退出
        if (strncmp(recv_buff, "quit", 4) == 0 || r <= 0) {
            printf("Clinet %d 退出聊天\n", idx);
            break;
        }
    }
}