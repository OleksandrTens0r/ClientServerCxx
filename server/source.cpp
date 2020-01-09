#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "Windows.h"
#include "Server.h"
#include "TaskFuture.h"
#include "ThreadPool.h"

int main()
{
    const unsigned short port_num = 3333;

    try 
    {
        Server srv;
        srv.start(port_num);

        // only for testing purposes
        std::this_thread::sleep_for(std::chrono::minutes(60));

        srv.stop();
    }
    catch (system::system_error &e) 
    {
        std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what();
    }

    return 0;
}