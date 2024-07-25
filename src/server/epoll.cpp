#include <sys/epoll.h>
#include "epoll.hpp"

// 将 服务器socket加入 epoll监听
bool s_add_epoll(struct epoll_event *event, int epoll_fd, int server_fd) {
    event->data.fd = server_fd;
    event->events = EPOLLIN;

    // EPOLL_CTL_ADD：注册新的fd到epfd中
    // EPOLL_CTL_MOD：修改已经注册的fd的监听事件
    // EPOLL_CTL_DEL：从epfd中删除一个fd
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, event) < 0) {
        std::cerr << "Failed to add server socket to epoll" << std::endl;
        close(server_fd);
        close(epoll_fd);
        return false;
    }
    return true;
    }
    


// int main(){
//     return 0;
// }
