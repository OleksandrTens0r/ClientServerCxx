#pragma once
#include <thread>
#include <future>
#include <queue>
#include "ThreadSafeQueue.h"
#include "ThreadTask.h"
#include "TaskFuture.h"

class ThreadPool
{
public:
    explicit ThreadPool(std::uint32_t num_threads = std::thread::hardware_concurrency());

    ThreadPool(const ThreadPool& rhs) = delete;
    ThreadPool& operator=(const ThreadPool& rhs) = delete;

    ~ThreadPool();

    template <typename Func, typename... Args>
    auto submit(Func&& func, Args&&... args)
    {
        auto bound_task = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
        using result_type = std::result_of_t<decltype(bound_task)()>;
        using packaged_task = std::packaged_task<result_type()>;
        using task_type = ThreadTask<packaged_task>;

        packaged_task task{ std::move(bound_task) };
        TaskFuture<result_type> result{ task.get_future() };
        m_work_queue.push(std::make_unique<task_type>(std::move(task)));
        return result;
    }

private:
    void worker();
    void destroy();

private:
    std::atomic_bool m_done;
    ThreadSafeQueue<std::unique_ptr<IThreadTask>> m_work_queue;
    std::vector<std::thread> m_threads;
};