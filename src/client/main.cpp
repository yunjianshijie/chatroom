//  #include "ui/ui.hpp"
#include "socket.hpp"
#include <cstring>
#include <string>
#include "json.hpp"
#include "threadpool.hpp"
// class Socket;
int main() {
    // 创建套接字，并检验传输
    Socket client(HAND); // 创建套接字并连接
    client.socket_do();
    return 0;
}
