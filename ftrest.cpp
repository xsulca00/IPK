#include <iostream>
#include <vector>
#include <regex>
#include <string>

#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#include <time.h>
#include <stdlib.h>

#include "bitstream.h"

using namespace std;

struct Remote_path
{
    string  server_name;
    int     port {};
    string  path;
};

struct Options
{
    string  command;
    Remote_path remote_path;
    string  local_path;
};

pair<Remote_path, bool> parse_remote_path(const string command)
{
    regex   pat {R"(http://(.+):([[:digit:]]+)/(.*))"};
    smatch  matches;

    if (regex_match(command, matches, pat))
    {
        if (3 < matches.size())
        {
            return {{matches[1], stoi(matches[2]), matches[3]}, true};
        }
    }
    return {{}, false};
}

Options get_options(int argc, char **argv)
{
    Options o;

    // ftrest command remote_path local_path
    if (argc >= 3)
    {
       o.command = argv[1];

       const auto res = parse_remote_path(argv[2]);
       if (res.second) 
       {
           Remote_path rm_path {res.first};

           o.remote_path.server_name = rm_path.server_name;
           o.remote_path.port = rm_path.port;
           o.remote_path.path = rm_path.path;
       }

       if (argc == 4)
       {
          o.local_path = argv[3];
       }
    }
    else
    {
        cerr << "Usage:\n" << argv[0] << " COMMAND REMOTE-PATH [LOCAL-PATH]\n";        exit(EXIT_FAILURE);
    }

    return o;
}

string build_msg(const string& cmd, const string& rmt_path, const string& host)
{
    char out[200] {};
    time_t t {time(NULL)};

    errno = 0;
    struct tm* tmp {localtime(&t)};

    if (tmp == NULL)
    {
        perror("localtime");
        exit(EXIT_FAILURE);
    }

    // get time in specified format
    if (strftime(out, sizeof(out), "%a, %m %b %Y %T %Z", tmp) == 0) 
    {
       cerr << "strftime returned 0";
       exit(EXIT_FAILURE);
    }

    return  cmd + " " + rmt_path + " " + "HTTP/1.1" + "\r\n"
          +"Host: " + host + "\r\n"
          +"Date: " + out + "\r\n"
          +"Accept: application/json" + "\r\n"
          +"Accept-Encoding: identity" + "\r\n"
          +"Content-Type: application/octet-stream" + "\r\n"
          +"\r\n"
          +"\r\n";
}

int main(int argc, char **argv)
{
    Options options {get_options(argc, argv)};
    int client_socket {-1}; 

    errno = 0;
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
    {
        perror("Creating socket error!");
        exit(EXIT_FAILURE);
    }

    errno = 0;
    struct hostent* server {gethostbyname(options.remote_path.server_name.c_str())};

    if (!server)
    {
        herror("Get host by name error!");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address {};

    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&server_address.sin_addr.s_addr,
          server->h_length);
    server_address.sin_port = htons(options.remote_path.port);


    errno = 0;
    if (connect(client_socket,
                (const struct sockaddr*) &server_address,
                sizeof(server_address)) != 0)
    {
        perror("Connect error!");
        exit(EXIT_FAILURE);
    }

    // connection established, now you can send data

    // type = file | folder
    // generate request
    const string& msg {build_msg(options.command, 
                                 "/"+options.remote_path.path+"?type=file", 
                                 server->h_name)};

    vector<char> bytes {get_bytes_from("s/homes/xsulca00/Stažené/elfutils-0.168.zip")};
   size_t size {bytes.size()};


    errno = 0;
    // sending bytes!
    const auto b1 {send(client_socket, msg.c_str(), msg.length(), 0)};
    if ( b1 < 0)
    {
        perror("Send error!");
        exit(EXIT_FAILURE);
    }

    char buf[1024] {};

    errno = 0;
    // recieving bytes!
    const auto b2 {recv(client_socket, buf, sizeof(buf), 0)};
    if ( b2 < 0)
    {
        perror("Send error!");
        exit(EXIT_FAILURE);
    }

    errno = 0;
    close(client_socket);

    return 0;
}
















