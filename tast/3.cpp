#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <string>

void sendMessages(std::promise<std::string> &promise) {
    int i = 0;
    try {
        while (true) {
            std::string message = "Message " + std::to_string(i++);
            promise.set_value(message);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (const std::exception &e) {
        promise.set_exception(std::current_exception());
    }
}

int main() {
    std::promise<std::string> promise;
    std::future<std::string> future = promise.get_future();

    // 启动异步任务
    std::thread messageThread(sendMessages, std::ref(promise));

    // 接收并输出消息
    while (true) {
        try {
            std::string message = future.get();
            std::cout << "Received message: " << message << std::endl;
        } catch (const std::future_error &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            break;
        } catch (const std::exception &e) {
            std::cerr << "Exception: " << e.what() << std::endl;
            break;
        }
    }

    // 等待线程结束
    messageThread.join();

    return 0;
}