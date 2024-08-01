#include "redis.hpp"

// 连接数据库

Redis::Redis() { //
    try {
        this->rc = redisConnect("127.0.0.1", 6379);
        //  连接本机
        if (this->rc->err) {
            redisFree(this->rc);
            throw std::runtime_error("连接redis服务器失败\n");
        }
        std::cout << "连接redis服务器成功" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}

std::string Redis::get_new_id() { // 获取新的ip
    try {
        std::string ip;
        redisReply *reply = (redisReply *)redisCommand(rc, "INCRBY ips 1");
        if (reply == nullptr) {
            throw std::runtime_error("redisCommand error");
        }
        if (reply->type != REDIS_REPLY_INTEGER) {
            freeReplyObject(reply);
            throw std::runtime_error("redisCommand error");
        }
        ip = std::to_string(reply->integer);
        freeReplyObject(reply);
        return ip;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    return "";
}

std::string Redis::setAccount(Account &account) { // 设置账号
    try {
        std::string new_id = get_new_id(); // 获取新的ip
        std::string key = "account:" + new_id;
        std::string com = "HMSET " + key + " name " + account.name + " password " + account.get_password() + " answer " + account.get_answer() + " question " + account.question;
        redisReply *reply = (redisReply *)redisCommand(rc, com.c_str());
        if (reply == nullptr) {
            throw std::runtime_error("redisCommand error");
        }
        if (reply->type != REDIS_REPLY_STATUS) {
            freeReplyObject(reply);
            throw std::runtime_error("redisCommand error");
        }
        if (strcmp(reply->str, "OK") != 0) {
            freeReplyObject(reply);
            throw std::runtime_error("redisCommand error");
        }
        return new_id;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return "";
    }
}
std::string Redis::getAccount(std::string id) { // 获取账号 返回账号josn信息

    try {
        std::string key = "account:" + id;
        std::string com = "HGETALL " + key;
        redisReply *reply = (redisReply *)redisCommand(rc, "HGETALL %s", key.c_str());
        if (reply == nullptr) {
            throw std::runtime_error("redisCommand error");
        }
        if (reply->type != REDIS_REPLY_ARRAY) {
            freeReplyObject(reply);
            throw std::runtime_error("redisCommand error");
        }
        std::string json = "{\"";
        for (int i = 0; i < reply->elements; i += 2) {
            json += reply->element[i]->str;
            json += "\":\"";
            json += reply->element[i + 1]->str;
            json += "\",\"";
            std::cout << reply->element[i]->str << ":" << reply->element[i + 1]->str << std::endl;
        }
        json.pop_back();
        json.pop_back();
        json += "}";
        freeReplyObject(reply);
        return json;

    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return "";
    }
    return "";
}
std::string Redis::delAccount(std::string &account) { // 删除账号
    return "";
}
std::string Redis::getAccountList() { // 获取账号列表
    return "";
}
std::string Redis::json_setAccount(std::string json) { // json设置账号 ,返回ip
    nlohmann::json j = nlohmann::json::parse(json);
    std::string id = get_new_id(); // 获取新的ip
    std::string key = "account:" + id;
    std::string value = j.dump();
    std::string name = j["name"];
    std::string password = j["password"];
    std::string answer = j["answer"];
    std::string question = j["question"];
    try {
        redisReply *reply;
        // 用哈希表存储账号信息
        std::string cmd = "HMSET " + key + " name " + name + " password " + password + " answer " + answer + " question " + question;
        // redisReply *reply = (redisReply *)redisCommand(rc, "SET %s %s", key.c_str(), json.c_str());
        reply = (redisReply *)redisCommand(rc, cmd.c_str());
        if (reply == nullptr) {
            throw std::runtime_error("redisCommand error");
        }
        if (reply->type != REDIS_REPLY_STATUS) {
            freeReplyObject(reply);
            throw std::runtime_error("redisCommand error");
        }
        if (strcmp(reply->str, "OK") != 0) {
            freeReplyObject(reply);
            throw std::runtime_error("redisCommand error");
        }
        freeReplyObject(reply);
        return id;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return "";
    }
}
std::string Redis::json_getAccount(std::string id) { // json获取账号
    Account account;
    try {
        redisReply *reply;
        std::string cmd = "HGETALL account:" + id;
        reply = (redisReply *)redisCommand(rc, cmd.c_str());
        if (reply == nullptr) {
            throw std::runtime_error("redisCommand error");
        }
        if (reply->type != REDIS_REPLY_ARRAY) {
            throw std::runtime_error("redisCommand error");
        }
        std::string pass;
        std::string answer;
        for (int i = 0; i < reply->elements; i += 2) { // elements 字段表示回复对象中包含的子元素的数量
            if (strcmp(reply->element[i]->str, "name") == 0) {
                account.name = reply->element[i + 1]->str;
                // std::cout << "name: " << account.name << std::endl;
            } else if (strcmp(reply->element[i]->str, "password") == 0) {
                pass = reply->element[i + 1]->str;
                account.set_pass(pass);
                // std::cout << "password: " << pass << std::endl;
                //  account.password = reply->element[i + 1]->str;
            } else if (strcmp(reply->element[i]->str, "answer") == 0) {
                answer = reply->element[i + 1]->str;
                account.set_answer(answer);
                //  std::cout << "answer: " << answer << std::endl;
                // account.answer = reply->element[i + 1]->str;
            } else if (strcmp(reply->element[i]->str, "question") == 0) {
                account.question = reply->element[i + 1]->str;
                // std::cout << "question: " << account.question << std::endl;
            }
        }
        freeReplyObject(reply);
        return account.toJsonString(-2);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return "";
    }
}
std::string Redis::json_delAccount(std::string json) { // json删除账号
    return "";
}
std::string Redis::json_getAccountList() { // json获取账号列表
    return "";
}

bool Redis::isAccountExist(std::string &account) {
    return true;
}
// int main() {
//     Redis redis;
//     // std::string json = "{\"name\":\"123\",\"password\":\"123\",\"answer\":\"123\",\"question\":\"123\"}";
//     // nlohmann::json j = nlohmann::json::parse(json); // json解析
//     // std::string value = j.dump();
//     // std::string name = j["name"];
//     // std::string password = j["password"];
//     // std::string answer = j["answer"];
//     // std::string question = j["question"];
//     // std::string id = redis.json_setAccount(json);
//     // std::cout << "id:" << id << std::endl;
//     // std::cout << "name:" << name << std::endl;
//     // std::cout << "password:" << password << std::endl;
//     // std::cout << "answer:" << answer << std::endl;
//     // std::cout << "question:" << question << std::endl;
//     std::string json2 = redis.json_getAccount("15");
//     std::cout << "json2:" << json2 << std::endl;
//     return 0;
// }