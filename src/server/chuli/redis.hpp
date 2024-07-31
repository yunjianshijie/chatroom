#include <iostream>
#include <string>
#include <cstring>
#include <hiredis/hiredis.h>
#include <cstddef>
#include "../../include/account.hpp"
#include "../../include/back.hpp"
class Redis {
    // 私密
private:
    redisContext *rc; // 只有一个
public:
    // 构造
    Redis();
    // 析构
    ~Redis() {
        redisFree(rc);
    }

    // redis命令
    // 用户 用哈希表存储
    void setAccount(Account &account);
 




};
