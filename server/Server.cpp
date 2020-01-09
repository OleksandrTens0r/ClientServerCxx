#include "Server.h"
#include "Acceptor.h"

void Server::start(unsigned short port_num)
{
    m_thread.reset(new std::thread([this, port_num]() { run(port_num); }));
}

void Server::stop()
{
    m_stop.store(true);
    if (m_thread->joinable())
    {
        m_thread->join();
    }
}

void Server::run(u_short port_num)
{
    Acceptor acc(m_ios, port_num);

    while (!m_stop.load())
    {
        acc.accept();
    }
}