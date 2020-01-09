#pragma once
#include <boost/asio/ip/tcp.hpp>
#include "ThreadPool.h"

class Service
{
public:
    Service();
    ~Service();

    void start_handling_client(std::shared_ptr<boost::asio::ip::tcp::socket> sock);

private:
    void handle_client(std::shared_ptr<boost::asio::ip::tcp::socket> sock) const;
    void handle_tasks();

    std::thread m_client_handler_thread;
    ThreadPool m_client_thread_pool;
    ThreadSafeQueue<TaskFuture<void>> m_client_tasks;

    std::shared_ptr<ThreadPool> m_task_thread_pool;
};
