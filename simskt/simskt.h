#ifndef SIMPIDBIT_SIMSKT_H
#define SIMPIDBIT_SIMSKT_H

#include <string>
#include <exception>
#include <algorithm>

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif

#ifdef __linux__
#include <sys/poll.h>
#include <sys/epoll.h>
#endif

/*

#ifdef _WIN32
#elif __linux__
#elif __APPLE__
#endif

*/
namespace simpid {

#ifdef _WIN32
    using   SocketHandle    = SOCKET;
#else
    using   SocketHandle    = int;
#endif

class Client {
public:
    SocketHandle    skt;
    std::string     ip;
    uint16_t        port;

    std::string     recvbuf;
    

    Client();
    ~Client();
    int send(std::string buf);
    int send(const char *buf, size_t length);

    std::string recv(size_t length = 1500);
    std::string recvall();
    int recv(char *buf, size_t length);
};

class Socket {
public:
    Socket(
        int     domain      = AF_INET,
        int     type        = SOCK_STREAM,
        int     protocol    = IPPROTO_TCP
    );
    ~Socket();

// Server
    int bind(std::string ip, uint16_t port);
    int listen(int backlog);
    Client accept();

// Client
    int connect(std::string ip, uint16_t port);

// Both
    int domain;
    int type;
    int protocol;
    SocketHandle skt;
    // size: BUFSIZ
    std::string recvbuf;

    int send(std::string buf);
    int send(const char *buf, size_t length);

    std::string recv(size_t length = 1500);
    std::string recvall();
    int recv(char *buf, size_t length);
};

namespace SocketException {
    class SystemNotReady : public std::exception { const char *what() noexcept; };
    class VersionNotSupported : public std::exception { const char *what() noexcept; };
    class TooManyProcesses : public std::exception { const char *what() noexcept; };
    class EventInProgress : public std::exception { const char *what() noexcept; };
    class UnknownError : public std::exception { const char *what() noexcept; };
    class VersionNotExist : public std::exception { const char *what() noexcept; };
    class InvalidSocket : public std::exception { 
    public:
        int errcode;
        char * strptr;

        InvalidSocket(int errcode);
        ~InvalidSocket();
        const char *what() noexcept;
    };
}

}

namespace spb       = simpid;
namespace Simpid    = simpid;
namespace SP        = simpid;
namespace SPB       = simpid;

#endif
