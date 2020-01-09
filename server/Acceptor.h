#pragma once
#include "Service.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>

class Acceptor
{
public:
    Acceptor(boost::asio::io_service& ios, u_short port_num);

    void accept();

private:
    boost::asio::io_service& m_ios;
    boost::asio::ip::tcp::acceptor m_acceptor;
    Service m_service;
};
