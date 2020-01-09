#include "ThreadPool.h"

ThreadPool::ThreadPool(const std::uint32_t num_threads) : m_done{ false }
{
    if (!num_threads)
    {
        throw std::runtime_error("hardware_concurrency func failed");
    }

    try
    {
        for (auto i = 0u; i < num_threads; ++i)
        {
            m_threads.emplace_back(&ThreadPool::worker, this);
        }
    }
    catch (...)
    {
        destroy();
        throw;
    }
}

ThreadPool::~ThreadPool()
{
    destroy();
}

void ThreadPool::worker()
{
    while (!m_done)
    {
        std::unique_ptr<IThreadTask> task{ nullptr };
        if (m_work_queue.wait_pop(task))
        {
            task->execute();
        }
    }
}

void ThreadPool::destroy()
{
    m_done = true;
    m_work_queue.invalidate();
    for (auto& thread : m_threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}