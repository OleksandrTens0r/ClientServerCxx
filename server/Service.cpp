#include "Service.h"
#include "Common.h"
#include "Helpers.h"
#include "Task.h"

#include <boost/asio/streambuf.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>

Service::Service()
{
    m_task_thread_pool = std::make_shared<ThreadPool>();
    m_client_handler_thread = std::thread(&Service::handle_tasks, this);
}

Service::~Service()
{
    if (m_client_handler_thread.joinable())
    {
        m_client_handler_thread.join();
    }
}

void Service::start_handling_client(std::shared_ptr<boost::asio::ip::tcp::socket> sock)
{
    m_client_tasks.push(m_client_thread_pool.submit([this, sock] { handle_client(sock); }));
}

void Service::handle_client(std::shared_ptr<boost::asio::ip::tcp::socket> sock) const
{
    try
    {
        boost::asio::streambuf request;
        read_until(*sock.get(), request, message_delimiter);

        if (is_session_alive(sock))
        {
            Task task(m_task_thread_pool, sock);
            const auto response = std::to_string(task.handle_task(get_string_from_buffer(request)));

            // possible listen client for new messages
            write(*sock.get(), boost::asio::buffer(response + message_delimiter));
        }
    }
    catch (boost::system::system_error& /* error */)
    {
        // possible error logging
    }
    catch (std::runtime_error& /* error */)
    {
        // session was canceled TODO create custom exception type
    }
}

void Service::handle_tasks()
{
    while (true)
    {
        TaskFuture<void> task = std::future<void>();
        m_client_tasks.wait_pop(task);
        task.get();
    }
}
