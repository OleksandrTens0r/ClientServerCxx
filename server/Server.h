#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include <thread>
#include <atomic>
#include <memory>

using namespace boost;

class Server
{
public:
    Server() : m_stop(false) { }

    void start(unsigned short port_num);

    void stop();

private:
    void run(u_short port_num);

    std::unique_ptr<std::thread> m_thread;
    std::atomic<bool> m_stop;
    asio::io_service m_ios;
};
