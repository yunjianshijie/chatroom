#include "redis.hpp"
#include <hiredis/hiredis.h>
#include <queue>
#include <thread>
#include <mutex>
#include <functional>
#include <future>
#include <cstddef>
// 连接数据库

// void openT(){
//     system("gnome-terminal --");
//     system("cd ~/redis/src");
//     system("redis-server &");
// }

Redis::Redis() {
    try {
        // std::thread t(openT); // 启动redis服务
        system("cd ~/redis/src");
        system("redis-server &");
        // 启动redis服务
        // system("gnome-terminal --");
        system("clear");
        system("clear");
        // 清屏

        this->rc = redisConnect("127.0.0.1", 6379);
        //  连接本机
        if (rc->err) {
            redisFree(rc);
            throw std::runtime_error("连接redis服务器失败\n");
        }
        std::cout << "连接redis服务器成功" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}

void Redis::setAccount(Account &account) {
}