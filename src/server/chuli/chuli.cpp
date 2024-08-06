#include "chuli.hpp"

// using json = nlohmann::json;
std::map<int, func> fun_map{
    {0, fun_bigin},
    {LOGIN, fun_login},
    {REGISTER, fun_register},
    {LOGIN_SUCCESS, fun_login_success},
    {USER_QUERY, fun_user_query},
    {FRIEND_ADD, fun_add_friend},
    {FRIEND_APPLY_LIST, fun_apply_friend},
    {FRIEND_APPLY, fun_friend_apply_result},
    {CHANGE_NAME, fun_change_name},
    {FRIEND_LIST, fun_friend_list},
 
};

// 这里是处理主函数
// 这里默认buffer为 josn
// 返回josn
std::string fanhui(std::string str, Redis &redis, int c_fd) {
    // std::cout << "buffer:" << buffer << std::endl;
    // std::string str(buffer);
    // std::cout << "str:" << str << std::endl;
    nlohmann::json j = nlohmann::json::parse(str);
    // 创建类型
    // 解析
    int mode = j["mode"];
    if (mode == 0) {
        j["c_fd"] = c_fd;
    }
    std::cout << "mode:" << mode << std::endl;
    return fun_init(mode, j, redis);
}

// 定义一个函数类型
typedef std::string (*func)(nlohmann::json &j, Redis &redis);
std::string fun_init(int mode, nlohmann::json &j, Redis &redis) {
    auto it = fun_map.find(mode);
    if (it != fun_map.end()) {
        return it->second(j, redis);
    } else {
        return "error";
    }
}

// 这里是处理josn
std::string fun_bigin(nlohmann::json &j, Redis &redis) {
    // std::string h = numToStr(0);
    nlohmann::json j1;
    j1["mode"] = 0;
    j1["c_fd"] = j["c_fd"];
    return j1.dump();
}

std::string fun_login(nlohmann::json &j, Redis &redis) {
    std::string h = numToStr(1);
    // h=1
    std::string TheId = j["id"];
    std::string ThePassword = j["password"];
    std::string jo = redis.getAccount(TheId);
    std::cout << "jo:" << jo << std::endl;
    return josn_str_login_ret(jo, ThePassword, TheId);
}

std::string fun_register(nlohmann::json &j, Redis &redis) {
    std::string h = numToStr(SUCCESS);
    // 将数据传入数据库
    Account account(j["name"], j["password"], j["question"], j["answer"]);
    // 设置账号
    std::string id = redis.setAccount(account);
    if (id == "") {
        return "error";
    }
    return josn_str_register(id);
}
// 登录成功
std::string fun_login_success(nlohmann::json &j, Redis &redis) {
    std::string id = j["id"];
    int fd = j["fd"];
    // 将哈希  fd（-1） 改成 fd
    // 退出后再改成 -1
    std::string com = "HSET account:" + id + " fd " + numToStr(fd);
    // 写进redis
    std::cout << "登录成功" << std::endl;
    redis.set_online(id, numToStr(fd));
    std::cout << "com:" << com << std::endl;
    redis.redis_command(com);
    return "";
}

std::string fun_change_name(nlohmann::json &j, Redis &redis) {
    std::string id = j["id"];
    std::string name = j["name"];
    std::string com = "HSET account:" + id + " name " + name;
    redis.redis_command(com);
    return "";
}

std::string josn_str_login(std::string y_n, Account account) { // 登录
    nlohmann::json j;
    std::cout << "dsfjdsklfsd" << std::endl;
    j["mode"] = LOGIN;
    // 处理是否有这个账号
    j["y_n"] = y_n;
    j["id"] = account.id;
    j["name"] = account.name;
    j["question"] = account.question;
    j["answer"] = account.get_answer();
    j["password"] = account.get_password();
    std::cout << "j:" << j.dump() << std::endl;
    return j.dump();
}
std::string josn_str_register(std::string id) {
    nlohmann::json j;
    j["mode"] = REGISTER;
    j["id"] = id;
    return j.dump();
}

std::string josn_str_chat(std::string id, std::string name, std::string message) {
    nlohmann::json j;
    // j["mode"] = CHAT;
    j["id"] = id;
    j["name"] = name;
    j["message"] = message;
    return j.dump();
}

std::string numToStr(int num) {
    return std::to_string(num);
}

std::string josn_str_login_ret(std::string josn, std::string ThePassword, std::string TheId) {
    std::string id;
    if (josn == "null") {
        printf("没有此账号\n");
        id = "-2";
        return josn_str_login("no", Account("", "", "", "", id));
    }
    nlohmann::json j1 = nlohmann::json::parse(josn);
    // std::cout << "j1:" << j1 << std::endl;
    printf("有此账号\n");
    std::string name = j1["name"];
    std::string anwser = j1["answer"];
    std::string password = j1["password"];
    std::string question = j1["question"];
    std::string fd = j1["fd"];
    id = TheId;
    if (fd != "-1") {
        std::cout << "该账号已登录" << std::endl;
        return josn_str_login("no", Account("", "", "", "", "-3"));
    }
    Account account(name, password, question, anwser, id);

    if (password != ThePassword) {
        std::cout << "密码错误" << std::endl;
        return josn_str_login("no", Account("", "", "", "", "-1"));
    }
    return josn_str_login("yes", account);
}

std::string fun_exit(nlohmann::json &j, Redis &redis) {
    return "";
    std::string id = j["id"];
    return redis.json_getUserInfo(id);
}

std::string fun_user_query(nlohmann::json &j, Redis &redis) {
    std::string id = j["id"];
    // id的格式为 account:id
    return redis.json_getUserInfo(id);
}
std::string fun_add_friend(nlohmann::json &j, Redis &redis) {
    printf("添加好友\n");
    std::string id = j["id"];
    std::string friend_id = j["friend_id"];
    std::string msg = j["msg"];
    std::string name = j["name"];
    std::string in = redis.json_getUserInfo(friend_id);
    nlohmann::json j1 = nlohmann::json::parse(in); // 查询好友信息
    std::cout << "in:" << in << std::endl;
    j1["mode"] = FRIEND_ADD;
    if (j1["fd"] == "-2") {
        printf("没有此账号\n");
    } else if (j1["fd"] == "-1") {
        std::cout << "好友不在线" << std::endl;
        // 先将它存入redis
        // 等待好友上线
    } else {
        std::cout << "好友在线" << std::endl;
        // 发送给好友
        // 好友的fd
        std::string fd1 = j1["fd"];
        std::cout << "size:" << in.size() << std::endl;
        // send(std::stoi(fd1), j1.dump().c_str(), j1.dump().size(), 0);
        nlohmann::json j2;
        j2["mode"] = FRIEND_ADD_2;
        j2["id"] = id;
        j2["msg"] = msg;
        j2["friend_id"] = friend_id;
        j2["name"] = name;
        send_fd(std::stoi(fd1), j2.dump());
        // 给好友的好友申请列表加上
        redis.apply_friend(friend_id, id, name, msg);
        return j1.dump();
    }
    // 先查用户存不存在
    // 存在后 用户在不在线
    // 在线则发送消息
    // 不在线则存入redis
    //    return redis.json_addFriend(id, friend_id);
    return j1.dump();
}
std::string fun_friend_apply_result(nlohmann::json &j, Redis &redis) {
    try {
        std::string id = j["id"];
        std::string friend_id = j["friend_id"];
        std::string chioce = j["chioce"];
        if (chioce == "1") {
            // 加好友
            redis.add_friend_list(id, friend_id);
            redis.add_friend_list(friend_id, id);
        }
        // 删申请
        redis.del_apply_friend(id, friend_id);
        return "";

    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    return "";
}

// 查看好友申请列表
std::string fun_apply_friend(nlohmann::json &j, Redis &redis) {
    std::string id = j["id"];
    return redis.get_apply_friend(id);
}
//
std::string fun_friend_list(nlohmann::json &j, Redis &redis) {
    std::string id = j["id"];
    return redis.get_friend_list(id);
}
//
std::string send_fd(int c_fd, std::string message) {
    try {
        int message_size = message.size();
        std::cout << "message_size:" << message_size << std::endl;
        std::cout << "message:" << message << std::endl;
        int ret = send(c_fd, message.c_str(), 1024, 0);
        std::cout << "send:" << ret << std::endl;

        if (ret == -1) {
            perror("send");
            return "error"; // 发送失败
        } else if (ret == 0) {
            return "error";
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    return "success";
}
