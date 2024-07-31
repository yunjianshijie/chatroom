#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <cstddef>
class ThreadPool {
public:
    ThreadPool(int num_threads); // 多少个线程
    ~ThreadPool();

    void enqueue(std::function<void()> task);

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks; // 任务对列
    std::mutex tasks_mutex;                  // 进程锁
    std::condition_variable condition;       //
    bool stop;

    void worker();
};
// 构造
ThreadPool::ThreadPool(int num_threads) :
    stop(false) { // 对stop的初始化
    for (int i = 0; i < num_threads; ++i) {
        workers.emplace_back(&ThreadPool::worker, this); //
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(tasks_mutex);
        stop = true;
    }
    condition.notify_all(); // 通知所有的线程
    for (std::thread &worker : workers) {
        worker.join(); // 等待所有线程结束
    }
}

void ThreadPool::enqueue(std::function<void()> task) { // 队列
    {
        std::unique_lock<std::mutex> lock(tasks_mutex);
        tasks.push(task); // 将任务添加到对列中
    }
    condition.notify_one(); // 通知一个正在等待该条件变量的线程
}

void ThreadPool::worker() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(tasks_mutex);                  
            condition.wait(lock, [this] { return !tasks.empty() || stop; }); // 如果队列为空，则等待
            if (stop && tasks.empty()) {
                return;
            }
            task = tasks.front(); // 将tesk 取出
            tasks.pop();
        }
        task(); // 执行任务
    }
}

// int main() {
//     ThreadPool pool(4);

//     for (int i = 0; i < 8; ++i) {
//         pool.enqueue([i] {
//             std::cout << "Task " << i << " executed by thread " << std::this_thread::get_id() << std::endl;
//         });
//     }

//     return 0;
// }