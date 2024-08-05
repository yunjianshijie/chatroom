#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

std::queue<std::string> message_queue;
std::mutex queue_mutex;
std::condition_variable queue_cv;

void send_messages() {
    while (true) {
        std::string message;
        std::cout << "Enter a message to send: ";
        std::getline(std::cin, message);
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            message_queue.push(message);
        }
        printf("sadasf");
        queue_cv.notify_one();
    }
}

void receive_messages() {
    while (true) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        queue_cv.wait(lock, [] { return !message_queue.empty(); });
        std::string message = message_queue.front();
        message_queue.pop();
        lock.unlock();

        std::cout << "Received message: " << message << std::endl;
    }
}

int main() {
    std::thread sender(send_messages);
    std::thread receiver(receive_messages);

    sender.join();
    receiver.join();

    return 0;
}