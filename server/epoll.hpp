#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <vector>


//将 服务器socket加入 epoll监听

bool s_add_epoll(struct epoll_event* event,int epoll_fd,int server_fd);

//将新请求加入epoll监听



