#pragma once
#include <boost/asio/ip/tcp.hpp>
#include "ThreadPool.h"

struct Response
{
    TaskFuture<uint64_t> future = std::future<uint64_t>();
    std::shared_ptr<boost::asio::ip::tcp::socket> sock;
};

class Service
{
public:
    Service();
    ~Service();

    void start_handling_client(std::shared_ptr<boost::asio::ip::tcp::socket> sock);

private:
    Response handle_client(std::shared_ptr<boost::asio::ip::tcp::socket> sock) const;
    void handle_tasks();

    std::thread m_client_handler_thread;
    std::thread m_tasks_result_handler_thread;

    ThreadPool m_client_thread_pool;
    ThreadSafeQueue<std::shared_ptr<TaskFuture<Response>>> m_client_tasks;

    std::shared_ptr<ThreadPool> m_fats_task_thread_pool;
    std::shared_ptr<ThreadPool> m_slow_task_thread_pool;
};
