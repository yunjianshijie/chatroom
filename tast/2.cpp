// #include <iostream>
// #include <nlohmann/json.hpp>
// #include <string>
// std::string numToStr(int num) {
//     return std::to_string(num);
// }
// std::string fanhui(const char *buffer, int num) {
//     // 这里是处理主函数
//     // 这里默认buffer为 josn
//     std::cout << "buffer:" << buffer << std::endl;
//     std::string str(buffer);

//     str = R"({"model": 1})";
//         std::cout << "str:" << str << std::endl;
//     nlohmann::json j = nlohmann::json::parse(str);
//     // 创建类型
//     // 解析
//     std::cout << "j:" << j << std::endl;
//     int mode = j["model"];
//     std::cout << "model:" << mode << std::endl;
//     if (mode == 1) {
//         // 如果是
//         std::string h = numToStr(1);
//         return h;
//     } else {
//         return "";
//     }
//     // return "hello";
// }

// int main() {
//     fanhui("{\"model\": 1}", 1);

//    //  假设我们有这个JSON数据字符串
//     std::string json_data = R"({"model": 1})";
// std::cout<< "json_data: " << json_data << std::endl;
//     // 使用nlohmann/json库解析JSON数据
//     nlohmann::json data = nlohmann::json::parse(json_data);

//     // 访问"model"属性的值
//     int model_value = data["model"];
//     std::cout << "Model value: " << model_value << std::endl; // 输出: Model value: 1

//     // 修改"model"属性的值
//     data["model"] = 2;
//     std::cout << data << std::endl; // 输出: {"model":2}

//     return 0;
// }

// 1. 创建通信的套接字
// int fd = socket(AF_INET, SOCK_STREAM, 0);
// if (fd == -1) {
//     perror("socket");
//     exit(0);
// }
// std::cout << "开始连接" << std::endl;
// // 2. 连接服务器
// struct sockaddr_in addr;
// addr.sin_family = AF_INET;
// addr.sin_port = htons(HAND); // 大端端口
// inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr.s_addr);

// int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
// if (ret == -1) {
//     perror("connect");
//     exit(0);
// }
// begin();
// std::cout << "你好" << std::endl;

// // 3. 和服务器端通信
// int number = 0;
// std::cout << "检测通信正常..." << std::endl;
// char test1[100] = "{\"mode\": 0}";
// //发送0
// ssize_t lee = send(fd, test1, sizeof(test1), 0);

// if (lee == -1) {
//     std::cerr << "Failed to send data" << std::endl;
//     printf("发送失败");
//     ssize_t lee = send(fd, test1, sizeof(test1), 0);
//     return 1;
// } else if (lee < sizeof(test1)) {
//     printf("发送少字");
//     return 1;
// }
// std::cout << "发送正常..." << std::endl;
// char buf[1024];
// memset(buf, 0, sizeof(buf));
// int len = read(fd, buf, sizeof(buf));
// if (len == -1) {
//     printf("接受失败");
//     return 1;
// }
// std::string mm = buf;
// main_t(mm);
// int s_mode = SUCCESS;

/// 这里是处理函数
// if (mode == 0) {
//     // 如果是
//     std::string h = numToStr(0);
//     nlohmann::json j;
//     j["mode"] = 0;
//     return j.dump();        // 返回json
// } else if (mode == LOGIN) { // 登录
//     std::string h = numToStr(1);
//     // h=1
//     std::string TheId = j["id"];
//     std::string ThePassword = j["password"];
//     std::string jo = redis.getAccount(TheId);
//     std::cout << "jo:" << jo << std::endl;
//     return josn_str_login_ret(jo, ThePassword, TheId);
// } else if (mode == REGISTER) // 注册
// {
//     std::string h = numToStr(SUCCESS);
//     // 将数据传入数据库
//     Account account(j["name"], j["password"], j["question"], j["answer"]);
//     // 设置账号
//     std::string id = redis.setAccount(account);
//     if (id == "") {
//         return "error";
//     }
//     return josn_str_register(id);
// }
// return "error";
// return "hello";
#include <iostream>
#include <thread>
#include <future>
#include <chrono>

// 线程B
void initiazer(std::promise<int> *promObj) {
    std::cout << "Thread B" << std::endl;
    // set the value at proper time
    std::this_thread::sleep_for(std::chrono::seconds(3));
    promObj->set_value(23);
}

int main() {
    // 线程A
    std::promise<int> promiseObj;
    std::future<int> futureObj = promiseObj.get_future();

    std::thread th(initiazer, &promiseObj); // 启动线程B

    // 获取对象的值，该调用在B设置其值后会返回23，在B设置其值前会阻塞
    std::cout << futureObj.get() << std::endl;

    th.join();

    return 0;
}

// std::future<int> computeResult() {
//     // 启动一个异步操作并返回结果
//     return std::async([] { return 42; });
// }

// int main() {
//     // 获取 future 对象
//     std::future<int> resultFuture = computeResult();

//     // 等待异步操作完成并获取结果
//     std::string q;
//     std::cin >> q;

//     int result = resultFuture.get();
//     std::cout << "Result: " << result << std::endl;

//     return 0;
// }
// 处理消息的线程函数



//为什么