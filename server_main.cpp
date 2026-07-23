#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>

#include "Server/Server.h"

int main(int argc, char **argv)
{
    Server server_{argv[1], argv[2], argv[3]};

    server_.bind_server();
    server_.listen_server(1);

    while (true)
    {
        server_.accept_client();

        while (true)
        {
            std::string line = server_.read_info();

            if (line.empty())
                break;

            std::cout << line << std::endl;
            server_.processStatistics(line);
        }

        server_.close_client_socket();
    }

    return 0;
}