#include <arpa/inet.h>
#include "epoll.hpp"
#include "socket.hpp"
#include "chuli/redis.hpp"
#include "chuli/chuli.hpp"
#include <hiredis/hiredis.h>
int main() {
    Redis redis; // 先创造一个redis对象
    // 通信
    Socket_con a;
    if (a.lfd < 0) {
        return 1;
    }
    // 用 epoll来多个客户端
    const int MAX_EVENTS = 40;
    struct epoll_event event, events[MAX_EVENTS];
    int epoll_fd = epoll_create1(0);
    std::vector<int> client_fds; // 客户端vector
    if (epoll_fd < 0) {
        std::cerr << "Failed to add socket to epoll" << std::endl;
        close(epoll_fd);
        return 1;
    }
    if (s_add_epoll(&event, epoll_fd, a.lfd) < 0) {
        return 1;
    }
    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); // 第三个参数是最大事件
        // nfds 这次调用中就绪的文件描述符的数量。
        if (nfds < 0) {
            std::cerr << "epoll_wait failed" << std::endl;
            continue;
        }
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == a.lfd) {
                // 有新的请求
                int client_fd = accept(a.lfd, nullptr, nullptr);
                if (client_fd < 0) {
                    std::cerr << "Failed to accept connection" << std::endl;
                    continue;
                }
                std::cout << "新的连接在自客户端 " << client_fd << std::endl;
                // 将新客户端 socket 加入 epoll 监听
                event.data.fd = client_fd;
                event.events = EPOLLIN;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) < 0) {
                    std::cerr << "Failed to add client socket to epoll" << std::endl;
                    close(client_fd);
                    continue;
                }
                client_fds.push_back(client_fd);
            } else {
                // 有数据到达
                char buffer[1024];
                memset(buffer, 0, 1024);
                ssize_t bytes_read = read(events[i].data.fd, buffer, 1024 - 1);
                if (bytes_read > 0) {
                    std::cout << "发来消息来自客户端 " << events[i].data.fd << ": " << buffer << std::endl;
                    // 现在收到传送的消息
                    std::string ret = fanhui(buffer, SUCCESS);

                    if (ret == "-1") {
                        std::cout << "解析失败" << std::endl;
                        continue;
                    }
                    std::cout << "发送 给 " << events[i].data.fd << " :" << ret << std::endl;

                    send(events[i].data.fd, ret.c_str(), sizeof(ret.c_str()), 0);
                    // 先直接原路返回
                    std::cout << "发送 给 " << events[i].data.fd << " :" << ret << std::endl;
                    // 发给客户端指令
                    // free(ret);
                    // ret = nullptr;
                    // 这个
                } else if (bytes_read == 0) {
                    std::cout << "Client " << events[i].data.fd << " disconnected" << std::endl;
                    //
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, nullptr) < 0) {
                        std::cerr << "Failed to remove client socket from epoll" << std::endl;
                    }
                    close(events[i].data.fd);
                    //   // client_fds.erase(std::remove(client_fds.begin(), client_fds.end(), events[i].data.fd), client_fds.end());
                    //   // 删除client_fds中的元素
                    //!!还没删
                } else {
                    std::cerr << "Failed to read from client socket" << std::endl;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, nullptr) < 0) {
                        std::cerr << "Failed to remove client socket from epoll" << std::endl;
                    }
                    close(events[i].data.fd);
                    //   auto it = std::find(client_fds.begin(), client_fds.end(), events[i].data.fd);
                    //   client_fds.erase(it, client_fds.end());
                }
            }
        }
    }
 
    return 0;
    //
    //    int cfd[40];
    //  int count1 = 0;

    //  cfd[count1] = a.Connect_clientfd();

    //  if (cfd[count1] != -1) {
    //      std::cout << "连接成功。 客户端文件描述符为 " << cfd[count1++] << " 。" << std::endl;
    //  } else {
    //      std::cout << "连接失败。" << std::endl;
    //      // break;
    //  }
    //  //
    //  while (1) {
    //      std::string str = a.get_new(cfd[count1 - 1]);
    //      if (str == "") {
    //          break;
    //      }
    //      std::cout << "客户端说" << str << std::endl;
    //      a.push_new(cfd[count1 - 1], "hello~");
    //  }

    //  sleep(30);
    // 解析
    // 分配线程处理
    // 返回有的值
    // return 0;
    return 0;
}
