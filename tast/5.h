#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <string.h>
#include <stdint.h>
#include <stdint.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <new>

#define MAX_PKT_SIZE (256 << 20) // 网络包最大长度
// 业务包头
struct CommMsgHdr {
    uint16_t uMsgType;
    uint32_t uTotalLen;
};

typedef struct _TcpHandle_ {
    int32_t fd;
    uint32_t uRcvLen; // 已接收数据大小
    uint32_t uAllLen; // 消息总长度

    struct sockaddr_in local_addr;
    struct sockaddr_in remote_addr;
    _TcpHandle_() {
        uRcvLen = 0;
        uAllLen = 0;
    }
} TcpHandle;

class TcpClient {
public:
    TcpClient();

    int32_t create_tcpClient(char *serverIp, int32_t serverPort);
    int32_t SendData(char *data, int32_t len);

    bool m_runing;
    int epoll_fd;
    TcpHandle *pTcpHandle;

private:
    pthread_t threadId;
};

#endif // TCPCLIENT_H
