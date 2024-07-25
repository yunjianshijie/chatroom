#include "redis.hpp"
#include<cstddef>
    // 连接数据库

    class redis {
public:
   // std::string command;
    redisContext *rc; //只有一个
    bool context(void) {
        redisContext *rc = redisConnect("127.0.0.1", 6379);
        // 连接本机
        if (rc->err) {
            redisFree(rc);
            printf("Connect to redisServer faile\n");
            return false;
        }
        printf("Connect to redisServer Success\n");
    }
    //redis命令
    bool Reply_command1(std::string command){
    redisReply *r = (redisReply *)redisCommand(rc, command.c_str());
    if (r == NULL) {
        std::cout << "Execut command1 failure\n";
        redisFree(rc);
        return false;
    }
    if (!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str, "OK") == 0)) {
        std:: cout <<"Failed to execute command["<< command <<"]\n";
        freeReplyObject(r);
        return false;
    }
    }
};
