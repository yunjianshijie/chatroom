#include "../include/back.hpp"
#include "json.hpp"
#include "../include/account.hpp"

std::string get_josn(std::string new_string, int num) {
    if (num == SUCCESS) {
        // 1 .登录
        // 2 .注册
        if (new_string == "1") { // 登录
            nlohmann::json j;
            j["mode"] = 1;
            return j.dump();
            // return "{\"mode\": 1}";
        } else if (new_string == "2") { // 注册
            return "{\"mode\": 2}";
        } else { //
            return "{\"mode\":-1}";
        }
    } else if (1) {
        return "{\"mode\":-1}";
    }
    return "{\"mode\": -1}";
}

std::string get_josn(Account &accout, int mode) {
    std::string str = accout.toJsonString(mode);
    return str;

    // return "";
}

// mode 是 int

// 将结构变成josn 字符串
// std::string structureToJson(const std::map<std::string, std::vector<std::string>> &structure) {
//     std::string json = "{";
//     for (const auto &item : structure) {
//         json += "\"" + item.first + "\":[";
//         for (const auto &value : item.second) {
//             json += "\"" + value + "\",";
//         }
//         json.pop_back(); // 删除最后一个逗号
//         json += "],";
//     }
//     json.pop_back(); // 删除最后一个逗号
//     json += "}";

//     return json;
// }