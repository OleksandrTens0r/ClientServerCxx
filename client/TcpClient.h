#pragma once

#include "Common.h"

#include <string>
#include <boost/asio.hpp>

struct Response
{
    std::string response_str;
    boost::system::error_code ec;
};

class TcpClient
{
public:
    TcpClient(const std::string& raw_ip_address, u_short port);
    void connect();
    void close();
    std::future<Response> perform_request(task_category category, uint64_t tasks_count);
    void cancel_session();

private:
    void send_request(std::string const& request);
    std::string get_string_from_buffer(const boost::asio::streambuf& stream_buff) const;
    Response receive_response();

    std::atomic<bool> m_is_session_canceled = false;
    boost::asio::io_service m_io_service;
    boost::asio::ip::tcp::endpoint m_end_point;
    boost::asio::ip::tcp::socket m_sock;
};
