#pragma once

#include <memory>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/ip/tcp.hpp>

std::string get_string_from_buffer(const boost::asio::streambuf& stream_buff);
bool is_session_alive(std::shared_ptr<boost::asio::ip::tcp::socket>& sock);
