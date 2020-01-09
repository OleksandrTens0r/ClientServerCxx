#include "TcpClient.h"

#include <string>

using namespace boost;

TcpClient::TcpClient(const std::string& raw_ip_address, u_short port)
    : m_end_point( asio::ip::address::from_string(raw_ip_address), port)
    , m_sock(m_io_service)
{
    m_sock.open(m_end_point.protocol());
}

void TcpClient::connect()
{
    m_sock.connect(m_end_point);
}

void TcpClient::close()
{
    m_sock.shutdown(asio::ip::tcp::socket::shutdown_both);
    m_sock.close();
}

void TcpClient::cancel_session()
{
    m_is_session_canceled = true;
}

void TcpClient::send_request(std::string const& request)
{
    write(m_sock, asio::buffer(request));
}

std::string TcpClient::get_string_from_buffer(const boost::asio::streambuf& stream_buff) const
{
    return std::string(buffers_begin(stream_buff.data()), buffers_begin(stream_buff.data()) + stream_buff.size());
}

Response TcpClient::receive_response()
{
    while (true)
    {
        asio::streambuf buf;
        read_until(m_sock, buf, message_delimiter);
        std::istream input(&buf);
        Response response;
        std::getline(input, response.response_str);

        if (response.response_str == check_session_message)
        {
            if (m_is_session_canceled)
            {
                send_request(session_canceled_message + message_delimiter);
                response.ec = asio::error::operation_aborted;
                return response;
            }

            send_request(session_alive_message + message_delimiter);
        }
        else
        {
            return response;
        }
    }
}

std::future<Response> TcpClient::perform_request(task_category category, uint64_t tasks_count)
{
    const auto request = to_str(category) +
        std::string(" data ") +
        std::string(data_delimiter) +
        std::to_string(tasks_count) +
        message_delimiter;
    send_request(request);
    return std::async(&TcpClient::receive_response, this);
}
