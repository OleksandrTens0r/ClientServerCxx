#pragma once

#include <utility>

class IThreadTask
{
public:
    IThreadTask() = default;
    virtual ~IThreadTask() = default;

    IThreadTask(const IThreadTask& other) = delete;
    IThreadTask& operator=(const IThreadTask& other) = delete;

    IThreadTask(IThreadTask&& other) = default;
    IThreadTask& operator=(IThreadTask&& other) = default;

    virtual void execute() = 0;
};

template <typename Func>
class ThreadTask : public IThreadTask
{
public:
    ThreadTask(Func&& func) : m_func(std::move(func)) { }

    ~ThreadTask() override = default;

    ThreadTask(const ThreadTask& rhs) = delete;
    ThreadTask& operator=(const ThreadTask& rhs) = delete;

    ThreadTask(ThreadTask&& other) = default;
    ThreadTask& operator=(ThreadTask&& other) = default;

    void execute() override
    {
        m_func();
    }

private:
    Func m_func;
};