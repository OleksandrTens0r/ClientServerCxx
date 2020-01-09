#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

template <typename T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue() = default;

    ThreadSafeQueue(const ThreadSafeQueue& other) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue& other) = delete;

    ~ThreadSafeQueue();

    bool try_pop(T& out);
    bool wait_pop(T& out);

    void push(T value);
    bool empty() const;
    void clear();
    void invalidate();
    bool is_valid() const;

private:
    std::atomic_bool m_valid{ true };
    mutable std::mutex m_mutex;
    std::queue<T> m_queue;
    std::condition_variable m_condition;
};


template <typename T>
ThreadSafeQueue<T>::~ThreadSafeQueue()
{
    invalidate();
}

template <typename T>
bool ThreadSafeQueue<T>::try_pop(T& out)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_queue.empty() || !m_valid)
    {
        return false;
    }

    out = std::move(m_queue.front());
    m_queue.pop();
    return true;
}

template <typename T>
bool ThreadSafeQueue<T>::wait_pop(T& out)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_condition.wait(lock, [this]()
        {
            return !m_queue.empty() || !m_valid;
        });

    if (!m_valid)
    {
        return false;
    }

    out = std::move(m_queue.front());
    m_queue.pop();
    return true;
}

template <typename T>
void ThreadSafeQueue<T>::push(T value)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push(std::move(value));
    m_condition.notify_one();
}

template <typename T>
bool ThreadSafeQueue<T>::empty() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.empty();
}

template <typename T>
void ThreadSafeQueue<T>::clear()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    while (!m_queue.empty())
    {
        m_queue.pop();
    }

    m_condition.notify_all();
}

template <typename T>
void ThreadSafeQueue<T>::invalidate()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_valid = false;
    m_condition.notify_all();
}

template <typename T>
bool ThreadSafeQueue<T>::is_valid() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_valid;
}