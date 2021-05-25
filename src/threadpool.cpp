#include "threadpool.h"

using std::cout;
using std::vector;
using std::exception;
using std::string;
using std::thread;
using std::mutex;

ThreadPool::ThreadPool(vector<string> wav_queue, bool (*fn)(const string&)) : queue(std::move(wav_queue)), work_fn(fn)
{
    ItemsTotal = queue.size();
    std::cout << "queue size #" << ItemsTotal << '\n';
    const size_t coreCount = thread::hardware_concurrency();
    std::cout << "coreCount #" << coreCount << '\n';
    NumThreads = std::min(ItemsTotal, coreCount);

    for (size_t i = 0; i < NumThreads; i++) {
        threads.emplace_back(std::thread(ThreadFunction, this));
    }
}


ThreadPool::~ThreadPool()
{
    for (size_t i = 0; i < NumThreads; i++) {
        if(threads.at(i).joinable())
        {
            threads.at(i).join();
            cout << "Successfully joined thread: " << i << '\n';
        }
    }
    cout << "Successfully processed: " << ItemsCompleted << "/" << ItemsTotal << " items" << '\n';
}

void ThreadPool::ThreadFunction(void *arg)
{
    auto *ctx = (ThreadPool*)arg;
    std::cout << "in thread #" << std::this_thread::get_id() << '\n';
    while (1) {
        std::lock_guard<std::mutex> lockGuard(ctx->queue_mutex);
        if (ctx->queue.empty())
        {
            break;
        }
        string cur(ctx->queue.back());
        ctx->queue.pop_back();      
        if (ctx->work_fn(cur)) // Increase counter on success
            ctx->ItemsCompleted++;
    }
    ctx = nullptr;
    return;
}
