#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool{
public:
    std::vector<std::thread> workers;
    std::atomic<bool> stop;
    std::atomic<int> total;
    std::queue<std::function<void()>> task_queue;
    std::mutex task_queue_mut;
    std::condition_variable task_queue_cond;

    ThreadPool(int size);
    template<typename T>
    void enqueue_task(T &&f);
    void main_thread();
    ~ThreadPool();
private:
    void worker_thread();
};

template<typename T>
void ThreadPool::enqueue_task(T &&f){
    std::lock_guard lk(task_queue_mut);
    task_queue.emplace(f);
    task_queue_cond.notify_one();
}

#endif