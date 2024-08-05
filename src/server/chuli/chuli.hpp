// #include"redis.hpp"
#include "redis.hpp"
#include <sys/socket.h>
typedef std::string (*func)(nlohmann::json &j, Redis &redis);
// std::map<int, func> fun_map;
std::string numToStr(int num);
std::string fanhui(char *buffer, Redis &redis,int c_fd);
// 登录josn 字符串
std::string josn_str_login(std::string id, Account account);
// 注册josn 字符串
std::string josn_str_register(std::string id);
// 聊天josn 字符串
std::string josn_str_chat(std::string id, std::string name, std::string message);
// 登录josn 返回字符串
std::string josn_str_login_ret(std::string josn, std::string ThePassword, std::string TheId);
//
std::string fun_init(int mode, nlohmann::json &j, Redis &redis);
//
std::string fun_bigin(nlohmann::json &j, Redis &redis);
std::string fun_login(nlohmann::json &j, Redis &redis);
std::string fun_register(nlohmann::json &j, Redis &redis);
std::string fun_login_success(nlohmann::json &j, Redis &redis);
//处理退出
std::string fun_exit(nlohmann::json &j, Redis &redis);
//用户查询
std::string fun_user_query(nlohmann::json &j, Redis &redis);
//添加好友
std::string fun_add_friend(nlohmann::json &j, Redis &redis);
//
//发送 
std::string send_fd(int c_fd, std::string message);
std::string fun_apply_friend(nlohmann::json &j, Redis &redis);