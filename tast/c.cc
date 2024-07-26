#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

#define BUFFER_SIZE 1024

void send_func();
void recv_func();

int clientSocket;

int main() {
    // 2. ����socket
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == -1) {
        perror("socket");
        exit(1);
    }

    // // 3. ��ȡ������Э���ַ��
    // struct sockaddr_in addr;
    // addr.sin_family = AF_INET; // Э��汾
    // addr.sin_addr.s_addr = inet_addr("192.168.179.128");
    // addr.sin_port = htons(1111); // �����ֽ���--->�����ֽ���

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1111); // 大端端口
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr.s_addr);

    // 4. ���ӷ�����
    int r = connect(clientSocket, (sockaddr *)&addr, sizeof(addr));
    if (r == -1) {
        perror("bind");
        exit(1);
    }

    // 5. ͨ��
    std::thread RecvMsg(&recv_func);
    RecvMsg.detach();
    send_func();

    return 0;
}

void send_func() {
    char send_buff[BUFFER_SIZE];

    while (1) {
        if (fgets(send_buff, BUFFER_SIZE, stdin) == NULL) {
            puts("no message\n");
            continue;
        } else {
            send(clientSocket, send_buff, sizeof(send_buff), 0);
            printf("I -----> Others : %s\n", send_buff);
        }

        if (strncmp(send_buff, "quit", 4) == 0) {
            break;
        }
    }
}

void recv_func() {
    char recv_buff[BUFFER_SIZE];
    while (1) {
        int r = recv(clientSocket, recv_buff, sizeof(recv_buff), 0);
        if (r > 0) {
            printf("Others -----> I : %s\n", recv_buff);
        }
    }
}