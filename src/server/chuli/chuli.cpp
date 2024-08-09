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
    {FRIEND_MESSAGE, fun_friend_char},
    {HISTORY_MESSAGE, fun_friend_history},
    {FRIEND_SHIELD, fun_friend_shield},
    {FRIEND_UNSHIELD, fun_friend_unshield},
    {EXIT, fun_exit1},
    {FRIEND_DELETE, fun_friend_delete},
    {GROUP_CREATE, fun_group_create},
    {GROUP_LIST, fun_group_list},
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
    // g o账号
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
    // 这里要返回s消息
    // 查询id_s_前缀的队列
    std::string key = id + "_s_*";
    std::cout << key << std::endl;
    std::vector<std::string> vec = redis.key_exist(key);
    nlohmann::json j1;
    j1["mode"] = LOGIN_SUCCESS;
    j1["id"] = id;
    j1["message"] = vec;
    std::cout << "j1:" << j1.dump() << std::endl;
    send_fd(fd, j1.dump());
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
        if (!redis.is_set("friend:" + id, friend_id)) {
            // 发送你已经被加好友了
            nlohmann::json j1;
            j1["mode"] = FRIEND_SHIELDED;
            j["or"] = "4";
            return j1.dump();
        }

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
std::string fun_friend_char(nlohmann::json &j, Redis &redis) {
    std::string id = j["id"];
    std::string friend_id = j["friend_id"];
    std::string msg = j["msg"];
    std::string fd = j["f_fd"];
    std::string send_id = j["send_id"]; // 发送者id
    std::time_t now = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
    std::string time_str = ss.str();
    nlohmann::json j1;
    j1["send_id"] = send_id;
    j1["id"] = friend_id;
    j1["friend_id"] = id;
    j1["msg"] = msg;
    j1["time"] = time_str;
    if (!redis.is_set("friend:" + id, friend_id)) {
        // 如果已经不是好友了那就只删聊天记录了
        nlohmann::json j1;
        j1["mode"] = FRIEND_DLEED;
        return j1.dump();
    };
    std::string ret = redis.getAccount(friend_id);
    std::cout << "ret:" << ret << std::endl;
    nlohmann::json j2 = nlohmann::json::parse(ret);

    std::string fd1 = j2["fd"];
    std::string key4 = "shield:" + friend_id;
    // 判断屏蔽
    nlohmann::json j3;
    j3["mode"] = FRIEND_SHIELDED;
    if (redis.is_set(key4, id)) {
        j3["or"] = "1";
        return j3.dump(); // 被屏蔽了
    }
    std::string key1 = "shield:" + id;
    printf("key1:%s\n", key1.c_str());
    if (redis.is_set(key1, friend_id)) {
        j3["or"] = "2";
        return j3.dump(); // 屏蔽了他
    }
    //
    //

    {
        std::string key = "frind:" + friend_id;
        if (redis.is_set(key, id)) {
            j3["or"] = "3";
            return j3.dump();
        }
    }
    if (fd1 != "-1") {
        // 给好友发送东西
        j1["mode"] = FRIEND_MESSAGE;
        // j1["fd"] = fd;
        // 这里应该时实获得fd，下线就是-1
        std::cout << "发送给 fd:" << fd1 << j1.dump() << std::endl;
        send_fd(std::stoi(fd1), j1.dump());
    } else {
        // 写进离线消息
        j1["mode"] = OFFLINE_MESSAGE;
        // std::string key = id + "_s_" + friend_id; // 私聊
        std::string key1 = friend_id + "_s_" + id;
        // redis.list_write(key, j1.dump());
        redis.list_write(key1, j1.dump());
        // 用有序列表加两个
    }
    j1["mode"] = HISTORY_MESSAGE;
    // redis.add_chat(id, friend_id, msg);
    std::string key2 = id + "_l_" + friend_id; // 私聊历史记录
    std::string key3 = friend_id + "_l_" + id; // 私聊历史记录
    redis.list_write(key3, j1.dump());
    redis.list_write(key2, j1.dump());
    return ""; // 不发给 id客户端发送东西
}
std::string fun_friend_history(nlohmann::json &j, Redis &redis) {
    std::string id = j["id"];
    std::string friend_id = j["friend_id"];
    std::string key = id + "_l_" + friend_id;
    std::vector<std::string> v = redis.list_read(key);
    // 读完把redis删掉
    std::string key1 = id + "_s_" + friend_id;
    std::cout << "key1:" << key1 << std::endl;
    redis.list_del(key1);
    nlohmann::json j1;
    j1["mode"] = HISTORY_MESSAGE;
    j1["history"] = v;
    std::cout << j1.dump() << std::endl;
    return j1.dump();
}

std::string send_fd(int c_fd, std::string message) {
    try {
        // int message_size = message.size();
        // std::cout << "message_size:" << message_size << std::endl;
        // std::cout << "message:" << message << std::endl;
        // int ret = send(c_fd, message.c_str(), 1024, 0);
        // std::cout << "send:" << ret << std::endl;
        int len = send_meg(c_fd, message);
        if (len == -1) {
            perror("send");
            return "error"; // 发送失败
        } else if (len == 0) {
            return "error";
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    return "";
}

std::string fun_friend_shield(nlohmann::json &j, Redis &redis) {
    // 设置shield:id 的集合
    std::string id = j["id"];
    std::string friend_id = j["friend_id"];
    std::string key = "shield:" + id;

    nlohmann::json j1;
    j1["mode"] = FRIEND_SHIELD;
    if (redis.is_set(key, friend_id)) {
        j1["y_n"] = "no";
    } else {
        j1["y_n"] = "yes";
        redis.add_set(key, friend_id);
    }
    return j1.dump();
}
std::string fun_friend_unshield(nlohmann::json &j, Redis &redis) {
    // 删除shield:id 的集合
    std::string id = j["id"];
    std::string friend_id = j["friend_id"];
    std::string key = "shield:" + id;
    nlohmann::json j1;
    j1["mode"] = FRIEND_UNSHIELD;
    if (redis.is_set(key, friend_id)) {
        j1["y_n"] = "yes";
        redis.del_set(key, friend_id);
    } else {
        j1["y_n"] = "no";
    }
    return j1.dump();
}
std::string fun_exit1(nlohmann::json &j, Redis &redis) {
    nlohmann::json j1;
    j1["mode"] = EXIT;
    std::string ret = redis.getAccount(j["id"]);
    nlohmann::json json = nlohmann::json::parse(ret);
    std::string fd = json["fd"];
    redis.set_offline(fd); // 设置为离线
    return j1.dump();
}
std::string fun_friend_delete(nlohmann::json &j, Redis &redis) {
    // 删掉数据库里面有关他的好友和屏蔽消息，和历史记录
    std::string id = j["id"];
    std::string friend_id = j["friend_id"];
    if (!redis.is_set("friend:" + id, friend_id)) {
        // 如果已经不是好友了那就只删聊天记录了
        nlohmann::json j1;
        j1["mode"] = FRIEND_DLEED;
        return j1.dump();
        std::string key1 = id + "_l_" + friend_id;
        redis.list_del(key1);
        std::string key2 = friend_id + "_l_" + id;
        redis.list_del(key2);
        std::string key3 = id + "_s_" + friend_id;
        redis.list_del(key3);
        std::string key4 = id + "_s_" + friend_id;
        redis.list_del(key4);
    };
    std::string key5 = id + "_l_" + friend_id;
    redis.list_del(key5);
    std::string key6 = friend_id + "_l_" + id;
    redis.list_del(key6);
    std::string key7 = id + "_s_" + friend_id;
    redis.list_del(key7);
    std::string key8 = id + "_s_" + friend_id;
    redis.list_del(key8);
    std::string key1 = "friend:" + id;
    redis.del_set(key1, friend_id);
    std::string key2 = "friend:" + friend_id;
    redis.del_set(key2, id);
    std::string key3 = "shield:" + id;
    redis.del_set(key3, friend_id);
    std::string key4 = "shield:" + friend_id;
    redis.del_set(key4, id);
    return "";
    // j["mode"] = FRIEND_DELETE;
    // j["id"] = account.id;
    // j["friend_id"] = id;
}

std::string fun_group_create(nlohmann::json &j, Redis &redis) {
    // 创建群聊
    // 获得id
    std::string group_id = redis.get_group_id();
    std::string group_name = j["name"];
    std::string group_owner = j["id"];
    // 将id放进redis
    redis.add_set("group_ids", group_id);
    std::string key = "group:" + group_id;
    redis.set_hash(key, "group_name", group_name);
    redis.set_hash(key, "group_owner", group_owner);
    // redis.set_hash(key, "group_members", "");
    // 对于个人管理群聊，和群聊群主
    // 用 集合
    key = "groupowner:" + group_owner;
    redis.add_set(key, group_id);
    // 加入的群聊
    key = "groupjoin:" + group_owner;
    redis.add_set(key, group_id);
    nlohmann::json j1;
    j1["mode"] = GROUP_CREATE;
    j1["group_id"] = group_id;
    return j1.dump();

    // 群聊用哈希表存储
    // 群聊id
    // 群聊名称
    // 群聊群主id
    // 群聊成员id //可空-> 现在是空的
    // 管理员id //可空-> 现在是空的
}

std::string fun_group_list(nlohmann::json &j, Redis &redis) {
    //    // 获得id
    std::string id = j["id"];
    std::string key = "groupjoin:" + id;
    std::vector<std::string> group_ids = redis.get_set(key);
    nlohmann::json j1;
    //写个集合把id放前面，名字放后面
    for (auto &group_id : group_ids) {
        std::string key1 = "group:" + group_id;
        std::string group_name = redis.get_hash(key1, "group_name");
    group_id+=group_name;
    }
    j1["mode"] = GROUP_LIST;
    j1["group_ids"] = group_ids;
    return j1.dump();
}