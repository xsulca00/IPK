#include <iostream>
#include <vector>

#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>


using namespace std;

struct Options
{
    string root_dir {"."};
    int port {80};
};

Options get_options(int argc, char **argv)
{
    Options o;

    for (int opt {}; (opt = getopt(argc, argv, "r:p:")) != -1;)
    {
       switch (opt)
       {
           case 'r':
               o.root_dir = optarg;
           case 'p':
               o.port = stoi(optarg);
               break;
           default:
               cerr << "Usage:\n" << argv[0] << " [-r ROOT_FOLDER] [-p PORT]\n";
       }
    }

    return o;
}

int main(int argc, char **argv)
{
    Options options {get_options(argc, argv)};
    int     welcome_socket {-1}; 

    errno = 0;
    if ((welcome_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
    {
        perror("Creating socket error!");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family       = AF_INET;
    server_address.sin_addr.s_addr  = INADDR_ANY;
    server_address.sin_port         = htons(options.port);

    int bs {-1}; 

    errno = 0;
    if ((bs = bind(welcome_socket,
                   (struct sockaddr*) &server_address,
                   sizeof(server_address))) < 0)
    {
        perror("Binding error!");
        exit(EXIT_FAILURE);
    }

    errno = 0;
    if (listen(welcome_socket, 1) < 0)
    {
        perror("Listen error!");
        exit(EXIT_FAILURE);
    }

    int comm_socket {-1};

    for(;;)
    {
        struct sockaddr_in  client {};
        socklen_t           client_len {sizeof(client)};

        comm_socket = accept(welcome_socket,
                             (struct sockaddr*) &client,
                             &client_len);

        if (comm_socket > 0)
        {
            for (int res {};;)
            {
                char buff[1024] {};
                // bytes recieved
                res = recv(comm_socket, buff, sizeof(buff), 0);
                if (res <= 0)
                {
                    break;
                }
                printf("%s", buff);
                send(comm_socket, buff, strlen(buff), 0);
            }
        }
    }

    close(comm_socket);
    close(welcome_socket);

    return 0;
}
















