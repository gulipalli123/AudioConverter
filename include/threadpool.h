#pragma once
#include <thread>
#include <vector>
#include <iostream>
#include <algorithm>
#include <mutex>

class ThreadPool {
    std::vector<std::thread> threads;
    std::vector<std::string> queue;
    static void ThreadFunction(void *arg);

    bool (*work_fn)(const std::string&);
    size_t ItemsTotal = 0;
    size_t ItemsCompleted = 0;
    size_t NumThreads = 0;
    std::mutex queue_mutex;
public:
    ThreadPool(std::vector<std::string> queue, bool (*fn)(const std::string&));
    ThreadPool() = delete;
    ~ThreadPool();
};
