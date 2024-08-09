#include "5.h"
//发消息正常发，包个头
//收东西处理一下
int32_t TcpRcv(const int32_t &fd, void *buff, const uint32_t &len) {
    int32_t iCurrRecv = recv(fd, buff, len, MSG_NOSIGNAL);
    if (0 < iCurrRecv) {
        return iCurrRecv;
    } else if (iCurrRecv < 0) {
        if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
            return 0;
        } else
            return -1;
    } else
        return -1;
}

void *DealTcpThread(void *obj) {
    TcpClient *pTcpClient = (TcpClient *)obj;
    TcpHandle *pTcpHandle = pTcpClient->pTcpHandle;

    const int kEpollDefaultWait = 1; // 超时时长，单位ms
    struct epoll_event alive_events[256];

    uint32_t recv_buffer_max = 1024 * 1024;
    uint8_t *recv_buffer = nullptr;
    recv_buffer = new uint8_t[recv_buffer_max];

    uint32_t head_len = (uint32_t)sizeof(CommMsgHdr);
    while (pTcpClient->m_runing) {
        int num = epoll_wait(pTcpClient->epoll_fd, alive_events, 256, kEpollDefaultWait);

        for (int i = 0; i < num; ++i) {
            int fd = alive_events[i].data.fd;
            int events = alive_events[i].events;

            if (events & EPOLLIN) {
                // 1.开始接收头部
                if (pTcpHandle->uRcvLen < head_len) {
                    int32_t iRecvLen = TcpRcv(fd, recv_buffer + pTcpHandle->uRcvLen, head_len - pTcpHandle->uRcvLen);
                    if (0 == iRecvLen)
                        continue;
                    else if (0 > iRecvLen) {
                        printf("Recv head data, return [%d] and err[%s],fd=[%d].", iRecvLen, strerror(errno), fd);
                        close(fd); // 关闭socket
                        continue;
                    }

                    pTcpHandle->uRcvLen += iRecvLen;

                    // 如果已经接收完整头部
                    if (pTcpHandle->uRcvLen >= head_len) {
                        CommMsgHdr *pHdr = (CommMsgHdr *)recv_buffer;
                        pTcpHandle->uAllLen = pHdr->uTotalLen;

                        // 如果报文头里的uTotalLen太小或太大，异常处理
                        if (pHdr->uTotalLen < head_len || pHdr->uTotalLen > MAX_PKT_SIZE) {
                            printf("uTotalLen invalid,uTotalLen=%u,fd=[%d]",
                                   pHdr->uTotalLen, fd);
                            close(fd); // 关闭socket
                            continue;
                        }

                        // 如果uTotalLen大于已分配的缓存，重新分配
                        if (((CommMsgHdr *)recv_buffer)->uTotalLen > recv_buffer_max) {
                            uint8_t *new_recv_buffer = new uint8_t[((CommMsgHdr *)recv_buffer)->uTotalLen];
                            memcpy(new_recv_buffer, recv_buffer, head_len);
                            delete[] recv_buffer;                                     // 释放原有空间
                            recv_buffer = new_recv_buffer;                            // 重新指向新开辟的空间
                            recv_buffer_max = ((CommMsgHdr *)recv_buffer)->uTotalLen; // 重新赋值最大buffer长度
                        }
                    }
                }
                // 2.开始接收数据体
                else {
                    int32_t iRecvLen = TcpRcv(fd, recv_buffer + pTcpHandle->uRcvLen, pTcpHandle->uAllLen - pTcpHandle->uRcvLen);
                    if (0 == iRecvLen)
                        continue;
                    else if (0 > iRecvLen) {
                        printf("Recv body data, return [%d] and err[%s],fd=[%d].", iRecvLen, strerror(errno), fd);
                        close(fd); // 关闭socket
                        continue;
                    }

                    pTcpHandle->uRcvLen += iRecvLen;
                    // 完成接收
                    if (pTcpHandle->uRcvLen == pTcpHandle->uAllLen) {
                        CommMsgHdr *pHdr = (CommMsgHdr *)recv_buffer;
                        printf("Rcv completed,msgType=%d,uTotalLen=%u\n", pHdr->uMsgType, pHdr->uTotalLen);
                        pTcpHandle->uRcvLen = 0;
                        pTcpHandle->uAllLen = 0;
                    }
                }
            }
        }
    }

    delete[] recv_buffer;
    recv_buffer = nullptr;

    return nullptr;
}

TcpClient::TcpClient() {
    pTcpHandle = new TcpHandle;
    epoll_fd = epoll_create(1);
}

int32_t TcpClient::create_tcpClient(char *serverIp, int32_t serverPort) {
    if (pTcpHandle == NULL) return -1;
    pTcpHandle->fd = -1;

    if ((pTcpHandle->fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket err=%s\n", strerror(errno));
        return -2;
    }

    pTcpHandle->remote_addr.sin_family = AF_INET;
    pTcpHandle->remote_addr.sin_port = htons(serverPort);
    pTcpHandle->remote_addr.sin_addr.s_addr = inet_addr(serverIp);
    if (connect(pTcpHandle->fd, (struct sockaddr *)&pTcpHandle->remote_addr, sizeof(pTcpHandle->remote_addr)) < 0) {
        printf("connect err=%s\n", strerror(errno));
        return -3;
    }

    struct epoll_event evt;
    evt.events = EPOLLIN;
    fcntl(pTcpHandle->fd, F_SETFL, O_NONBLOCK); // 设置非阻塞
    evt.data.fd = pTcpHandle->fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, pTcpHandle->fd, &evt);

    m_runing = true;
    pthread_create(&threadId, NULL, DealTcpThread, this);

    return 0;
}

int32_t TcpClient::SendData(char *data, int32_t len) {
    int32_t ret = send(pTcpHandle->fd, data, len, MSG_NOSIGNAL);
    return ret;
}
