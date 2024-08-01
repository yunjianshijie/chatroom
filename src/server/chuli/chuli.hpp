// #include"redis.hpp"
#include "redis.hpp"
std::string numToStr(int num);
std::string fanhui(char *buffer, int num, Redis &redis);

std::string josn_str_login(std::string id,Account account);                                       // 登录josn 字符串
std::string josn_str_register(std::string id);                                    // 注册josn 字符串
std::string josn_str_chat(std::string id, std::string name, std::string message); // 聊天josn 字符串

std::string josn_str_login_ret(std::string josn, std::string ThePassword,std::string TheId); // 登录josn 返回字符串
