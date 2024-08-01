#include <iostream>
#include <string>
#include <cstring>
#include <hiredis/hiredis.h>
#include <cstddef>
#include <queue>
#include <thread>
#include <mutex>
#include <functional>
#include <future>
#include "../../include/account.hpp"
#include "../../include/back.hpp"
#include <nlohmann/json.hpp>
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
    std::string get_new_id();                      // 获取新ip
    std::string setAccount(Account &account);      // 设置账号
    std::string getAccount(std::string id);        // 获取账号
    std::string delAccount(std::string &account);  // 删除账号
    std::string getAccountList();                  // 获取账号列表
    std::string json_setAccount(std::string json); // json设置账号
    std::string json_getAccount(std::string id);   // json获取账号
    std::string json_delAccount(std::string json); // json删除账号
    std::string json_getAccountList();             // json获取账号列表
    bool isAccountExist(std::string &account);     // 判断账号是否存在

};
