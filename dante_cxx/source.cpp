#include <mutex>
#include <iostream>
#include <vector>
#include "../server/TaskFuture.h"
#include "../server/ThreadPool.h"

int lengthyProcessWithArguments(int a, int b)
{
    return a + b;
}

std::mutex mtx;

void foo()
{
    static unsigned counter = 0;
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << ++counter << std::endl;
}

namespace default_thread_pool
{
    inline ThreadPool& get_thread_pool()
    {
        static ThreadPool defaultPool;
        return defaultPool;
    }

    template <typename Func, typename... Args>
    auto submit_job(Func&& func, Args&&... args)
    {
        return get_thread_pool().submit(std::forward<Func>(func), std::forward<Args>(args)...);
    }
}


int main()
{
    std::vector<TaskFuture<void>> v;
    for (std::uint32_t i = 0u; i < 100u; ++i)
    {
        v.push_back(default_thread_pool::submit_job([]()
            {
                foo();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }));
    }
    for (auto& item : v)
    {
        item.get();
    }
    system("pause");
}