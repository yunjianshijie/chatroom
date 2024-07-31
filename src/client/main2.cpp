// #include "ui/ui.hpp"
// #include "socket.hpp"
// #include <cstring>
// #include <string>
// #include "json.hpp"
// #include "threadpool.hpp"
// // class Socket;

// void recv_func1(Socket &client);       //
// std::queue<std::string> message_queue; // 消息队列
// std::mutex queue_mutex;                // 互斥锁
// std::condition_variable queue_cv;      // 条件变量

// int main() {
//     // 创建套接字，并检验传输
//     Socket client(HAND); // 创建套接字并连接
//     Account my_account;  // 创建线程账号
//     // ThreadPool pool(10); // 创建线程池
//     // 创建一个线程 接收消息

//     //  接收数据
//     // std::string c_get = ""; // 不用这样
//     // 用消息队列
//     std::thread RecvMsg(std::ref(recv_func1), std::ref(client)); // 创建线程
//     // 由于std::thread 要求传入的函数参数必须都是可以转换为右值引用的
//     RecvMsg.detach();
//     printf("服务器发来: %s\n", message_queue.front().c_str());
//     while (1) {
//         // 发送数据
//         // pool.enqueue([&client, &my_account] {
//         // });
//         std::string xinxi = client.main_t();
//         if (client.r_just()) {
//             xinxi = client.main_t(my_account);
//         }
//         // 输入还没有
//         // 靠服务器上次发来的信号，来判断现在在哪里
//         std::cout << "发送的数据: " << xinxi << std::endl;
//         std::unique_lock<std::mutex> lock(queue_mutex); // 将消息队列上锁
//         if (client.send_string(xinxi)) {
//             printf("发送成功\n");
//         }
//     }

//     // 关闭套接字
//     //  close(fd);
//     return 0;
// }

// void recv_func1(Socket &client) { // std::ref 包装
//         printf("1zzz\n");
//     while (1) {
//         std::unique_lock<std::mutex> lock(queue_mutex); // 将消息队列上锁
//         printf("正在接收消息\n");
//         message_queue.push(client.receive_string()); // 接收消息
//         std::cout << "接收的数据: " << message_queue.back() << std::endl;
//         printf("z2zzz");
//         queue_cv.wait(lock, [] { return !message_queue.empty(); });
//         // 等队列有东西
//         lock.unlock(); // 解锁
//         printf("3zzz");
//         std::string c_get1;
//         if (!message_queue.empty()) {
//             c_get1 = message_queue.front(); // 第一条
//             // c_get1 = client.receive_string();
//             // if (c_get1 == "") {
//             //     return;
//             // }
//             std::cout << "服务器说:" << c_get1 << std::endl;
//         }
//     }
// }
