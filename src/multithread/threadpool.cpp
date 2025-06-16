#include "threadpool.h"

void ThreadPool::main_thread(){
    while(true){
        std::function<void()> task;
        {
            std::lock_guard lk(task_queue_mut);
            if(task_queue.empty()) break;
            task = task_queue.front();
            task_queue.pop();
        }
        task();
        // total += 1;
    }

    // std::cout << "total: " << total << std::endl;
}
void ThreadPool::worker_thread(){
    while(true){
        std::function<void()> task;
        {
            std::unique_lock lk(task_queue_mut);
            task_queue_cond.wait(lk, [this]{ return !this->task_queue.empty() || stop; });
            if(task_queue.empty() && stop) break;
            task = task_queue.front();
            task_queue.pop();
        }
        task();
        // total += 1;
    }
}
ThreadPool::ThreadPool(int size){
    total = 0;
    stop = false;
    for(int i = 0; i < size; i++){
        workers.emplace_back(&ThreadPool::worker_thread, this);
    }
}
ThreadPool::~ThreadPool(){
    stop = true;
    task_queue_cond.notify_all();
    for(int i = 0; i < workers.size(); i++){
        workers[i].join();
    }
    std::cout << "destroy: " << std::endl;
}