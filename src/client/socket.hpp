// // #include <iostream>
// // #include <string>
// // #include <arpa/inet.h>
// // #include <sys/types.h>
// // #include <sys/socket.h>
// // #include <netinet/in.h>
// // #include <cstring>
// // #include <optional>
// // #include <stdlib.h>
// // #include <unistd.h>
// // #include "../include/back.hpp"
// // #include <nlohmann/json.hpp>
// // #include "ui/ui.hpp"
// #include "user.hpp"
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <optional>
#include <stdlib.h>
#include <unistd.h>
#include "../include/back.hpp"
#include <nlohmann/json.hpp>
#include "ui/ui.hpp"
#include <queue>
#include <thread>
#include <future>
class Socket {
private:
    /* data */

    struct sockaddr_in addr; //
    int just;                // 如果是1，则代表非用户操作, 如果是2，则代表用户操作
    Account account;
    int strToNum(std::string str) {
        std::stringstream ss(str);
        // 如果传回来不是数字就用josn读回来
        int num = 0;
        ss >> num;
        return num;
    }
    // std::queue<std::string> messageQueue;
    // std::mutex queueMutex;
    // std::condition_variable queueCondVar;
    // 消息队列
    std::queue<std::string> message_queue;
    // 队列互斥锁
    std::mutex queue_mutex;
    // 条件变量
    std::condition_variable queue_cv;
    // 线程结束标志
    std::mutex mtx;
    std::condition_variable cv;
    bool result_ready = false;

public:
    int server_fd;
    std::string buf;                 // 服务器传回来的信息
    int client_fd;                   // 客户端ip
    int mode;                        // mode 记录之后的操作
    Socket(int handle, char ip[30]); // handle 是服务器端口号
    ~Socket() {
        close(client_fd); // 关闭客户端的ip
    }
    // 处理主要逻辑
    void socket_do();
    // 发送数据
    bool send_string(std::string chuan);
    // 接受数据
    std::string receive_string();
    // 发送注册json数据
    bool send_json_usr_register(Account &account);
    // 接收注册json数据
    std::string receive_json_usr_id();
    // 记住id
    bool remember_id(std::string id);
    // 接收登录json数据
    Account receive_json_usr_login();
    // 发送登录json数据
    std::string send_json_usr_login(Account &account);
    // 退出登录json数据
    std::string send_json_usr_exit();
    // 登录成功发送
    std::string send_josn_login_success();
    // 错误信息
    bool error_string(std::string str);
    // 修改密码
    bool change_pass(Account &account);

    // 结束在Usr
    // 进入用户界面
    void user_run();
    // 查看自己信息1
    void showSelfInfo();
    // 查询用户2
    void queryUser();
    // 查看好友列表3
    void showFriendList();
    // 查看群聊列表4
    void showGroupList();
    // 添加好友5
    void addFriend();
    // 查看好友申请6
    void apply_FriendList();
    // 查看群聊申请7
    void apply_GroupList();
    // 添加群聊8
    void addGroup();
    // 创建群聊9
    void createGroup();
    // 查看聊天记录10
    void showChatRecord();
    // 注销账号11
    bool cancel();
    // 好友
    std::string send_json_user_que(std::string id); // 发送添加好友请求
    // 修改昵称
    void send_change_name(std::string name);
    //
    std::string send_json_friend_list();
    // 接受消息
    std::string receive_message();
    // 线程接收消息
    void receive_json();
    //
    void receive_josn_user(std::string massage);

    // 发送
    // 发送好友同样消息
    std::string send_json_friend_apply(std::string id, std::string friend_id, std::string chioce);

    // 打印用户信息
    void print_user_qu(std::string massage);
    void print_friend_add(std::string message);
    void print_friend_add_2(std::string message);
    void print_friend_apply_list(std::string message);           // 打印好友申请列表
    void print_friend_apply_list2(std::vector<std::string> vec); // 打印好友申请列表2
    void print_friend_list(std::string message);                 // 打印好友列表
};
