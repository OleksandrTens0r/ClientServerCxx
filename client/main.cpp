#include "TcpClient.h"
#include "Common.h"

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <queue>

const std::string raw_ip_address = "127.0.0.1";
const u_short port_num = 3333;

std::string emulate_client(uint64_t task_count)
{
    TcpClient client(raw_ip_address, port_num);
    client.connect();

    const auto response = client.perform_request(task_category::fast, task_count).get();
    if (response.ec == boost::asio::error::operation_aborted)
    {
        return "session was canceled";
    }

    client.close();
    return response.response_str;
}

int main()
{
    try
    {
        std::queue<std::future<std::string>> results;
        for (auto i = 0u; i < 100; ++i)
        {
            results.emplace(std::async([i]() { return emulate_client(i); }));
        }

        while (!results.empty())
        {
            std::cout << results.front().get() << std::endl;
            results.pop();
        }

    }
    catch (const std::exception& exception)
    {
        std::cout << "error occured while processing " << exception.what() << std::endl;
    }

    system("pause");
}
