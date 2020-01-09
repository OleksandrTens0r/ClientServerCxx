#include "Acceptor.h"

Acceptor::Acceptor(boost::asio::io_service& ios, u_short port_num)
    : m_ios(ios)
    , m_acceptor(m_ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port_num))
{
    m_acceptor.listen();
}

void Acceptor::accept()
{
    std::shared_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(m_ios));

    m_acceptor.accept(*sock.get());

    m_service.start_handling_client(sock);
}