#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
std::string numToStr(int num) {
    return std::to_string(num);
}
std::string fanhui(const char *buffer, int num) {
    // 这里是处理主函数
    // 这里默认buffer为 josn
    std::cout << "buffer:" << buffer << std::endl;
    std::string str(buffer);

    str = R"({"model": 1})";
        std::cout << "str:" << str << std::endl;
    nlohmann::json j = nlohmann::json::parse(str);
    // 创建类型
    // 解析
    std::cout << "j:" << j << std::endl;
    int mode = j["model"];
    std::cout << "model:" << mode << std::endl;
    if (mode == 1) {
        // 如果是
        std::string h = numToStr(1);
        return h;
    } else {
        return "";
    }
    // return "hello";
}

int main() {
    fanhui("{\"model\": 1}", 1);

   //  假设我们有这个JSON数据字符串
    std::string json_data = R"({"model": 1})";
std::cout<< "json_data: " << json_data << std::endl;
    // 使用nlohmann/json库解析JSON数据
    nlohmann::json data = nlohmann::json::parse(json_data);

    // 访问"model"属性的值
    int model_value = data["model"];
    std::cout << "Model value: " << model_value << std::endl; // 输出: Model value: 1

    // 修改"model"属性的值
    data["model"] = 2;
    std::cout << data << std::endl; // 输出: {"model":2}

    return 0;
}

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