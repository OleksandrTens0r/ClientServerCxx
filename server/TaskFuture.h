#pragma once

#include <future>

template <typename T>
class TaskFuture
{
public:
    TaskFuture(std::future<T>&& future) : m_future(std::move(future))
    {
    }

    TaskFuture(const TaskFuture& rhs) = delete;
    TaskFuture& operator=(const TaskFuture& rhs) = delete;

    TaskFuture(TaskFuture&& other) = default;
    TaskFuture& operator=(TaskFuture&& other) = default;

    ~TaskFuture()
    {
        if (m_future.valid())
        {
            m_future.get();
        }
    }

    auto get()
    {
        return m_future.get();
    }

private:
    std::future<T> m_future;
};