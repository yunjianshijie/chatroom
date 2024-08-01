#include "chuli.hpp"

// using json = nlohmann::json;

std::string fanhui(char *buffer, int num, Redis &redis) {
    // 这里是处理主函数
    // 这里默认buffer为 josn
    std::cout << "buffer:" << buffer << std::endl;
    std::string str(buffer);
    // std::cout << "str:" << str << std::endl;
    nlohmann::json j = nlohmann::json::parse(str);
    // 创建类型
    // 解析
    int mode = j["mode"];
    std::cout << "mode:" << mode << std::endl;

    // 这里是处理函数
    if (mode == 0) {
        // 如果是
        std::string h = numToStr(0);
        nlohmann::json j;
        j["mode"] = 0;
        return j.dump();        // 返回json
    } else if (mode == LOGIN) { // 登录
        std::string h = numToStr(1);
        // h=1
        std::string TheId = j["id"];
        std::string ThePassword = j["password"];
        std::string jo = redis.getAccount(TheId);
        std::cout << "jo:" << jo << std::endl;
        return josn_str_login_ret(jo, ThePassword, TheId);
    } else if (mode == REGISTER) // 注册
    {
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
    return "error";
    // return "hello";
}

// 这里是处理josn

std::string josn_str_login(std::string y_n, Account account) { // 登录
    nlohmann::json j;
    std::cout << "dsfjdsklfsd"<< std::endl;
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
    //  j["mode"] = CHAT;
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
    if (josn == "}") {
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
    id = TheId;
    Account account(name, password, question, anwser, id);
    if (password != ThePassword) {
        std::cout << "密码错误" << std::endl;
        return josn_str_login("no", Account("", "", "", "", "-1"));
    }
    return josn_str_login("yes", account);
}