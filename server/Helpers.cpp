#include "Helpers.h"
#include "Common.h"

#include <boost/asio/write.hpp>
#include <boost/asio/read_until.hpp>

std::string get_string_from_buffer(const boost::asio::streambuf& stream_buff)
{
    return std::string(buffers_begin(stream_buff.data()), buffers_begin(stream_buff.data()) + stream_buff.size());
}

bool is_session_alive(std::shared_ptr<boost::asio::ip::tcp::socket>& sock)
{
    write(*sock.get(), boost::asio::buffer(check_session_message + message_delimiter));
    boost::asio::streambuf response_buff;
    read_until(*sock.get(), response_buff, message_delimiter);
    if (get_string_from_buffer(response_buff) == session_alive_message + message_delimiter)
    {
        return true;
    }

    return false;
}