#pragma once

#include "ThreadPool.h"
#include "Common.h"
#include "Helpers.h"

#include <boost/asio/ip/tcp.hpp>

class Task
{
public:
    Task(std::shared_ptr<ThreadPool> thread_pool, std::shared_ptr<boost::asio::ip::tcp::socket> socket);
    uint64_t handle_task(const std::string& request);

private:
    void check_session();
    uint64_t emulate_task(uint32_t number);

    static task_category get_task_type(const std::string& request_string);
    static uint32_t get_data_from_request_string(const std::string& request);

private:
    std::shared_ptr<ThreadPool> m_task_thread_pool;
    std::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
    std::mutex m_socket_mtx;
};
