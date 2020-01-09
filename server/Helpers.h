#pragma once

#include <memory>
#include <future>

#include <boost/asio/streambuf.hpp>
#include <boost/asio/ip/tcp.hpp>

std::string get_string_from_buffer(const boost::asio::streambuf& stream_buff);
bool is_session_alive(std::shared_ptr<boost::asio::ip::tcp::socket>& sock);

template<typename R>
bool is_ready(std::future<R> const& f)
{
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}