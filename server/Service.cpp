#include "Service.h"
#include "Common.h"
#include "Helpers.h"
#include "Task.h"

#include <boost/asio/streambuf.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>

Service::Service()
{
    m_fats_task_thread_pool = std::make_shared<ThreadPool>();
    m_slow_task_thread_pool = std::make_shared<ThreadPool>();
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
    m_client_tasks.push(std::make_shared<TaskFuture<Response>>(m_client_thread_pool.submit([this, sock] { return handle_client(sock); })));
}

Response Service::handle_client(std::shared_ptr<boost::asio::ip::tcp::socket> sock) const
{
    try
    {
        boost::asio::streambuf request;
        read_until(*sock.get(), request, message_delimiter);

        if (is_session_alive(sock))
        {
            Task task(m_fats_task_thread_pool, m_slow_task_thread_pool, sock);
            return { task.handle_task(get_string_from_buffer(request)), sock};
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

    return { TaskFuture<uint64_t>(std::future<uint64_t>()), sock};
}

void Service::handle_tasks()
{
    while (true)
    {
        std::shared_ptr<TaskFuture<Response>> task;
        if (m_client_tasks.wait_pop(task))
        {
            if (task.get()->is_ready())
            {
                auto response = task.get()->get();
                const auto data = response.future.get();
                write(*response.sock.get(), boost::asio::buffer(std::to_string(data) + message_delimiter));
            }
            else
            {
                m_client_tasks.push(task);
            }

        }
    }
}