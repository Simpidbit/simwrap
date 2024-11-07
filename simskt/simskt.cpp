/*
   linux下程序的返回值:
    1 - socket() 出错
    2 - bind() 出错
    3 - listen() 出错
    4 - accept() 出错
    5 - connect() 出错
    6 - send() 出错
    7 - recv() 出错
*/

#include "simskt.h"

// 出错时是否抛出异常
bool IF_THROW = true;

#define DEFINE_EXCEPTION_WHAT(EXCEPTION_NAME, WHATSTR) \
    const char * simpid::SocketException::EXCEPTION_NAME::what() noexcept { return WHATSTR; }
DEFINE_EXCEPTION_WHAT(SystemNotReady, "System was not ready, check your system settings, win32 library or environment configuration. Error code: 10091.")
DEFINE_EXCEPTION_WHAT(VersionNotSupported, "Version was not supported by your network library.")
DEFINE_EXCEPTION_WHAT(TooManyProcesses, "Too many processes, close some of them and try again.")
DEFINE_EXCEPTION_WHAT(EventInProgress, "Event in progress, the event you requested had been in progress.")
DEFINE_EXCEPTION_WHAT(UnknownError, "Unknown error, I don\'t know what happened!")
DEFINE_EXCEPTION_WHAT(VersionNotExist, "Version not exists. Make sure your version is valid. Maybe you can try to modify the variable \"version_required\" in \"simskt.cpp\".")

simpid::SocketException::InvalidSocket::InvalidSocket(int errcode) : errcode(errcode) {}
const char * simpid::SocketException::InvalidSocket::what() noexcept
{
    std::string tmpstr = (std::string)"Invalid socket: error code = " + std::to_string(this->errcode);
    this->strptr = new char(tmpstr.size() + 1);
    memcpy(this->strptr, tmpstr.c_str(), tmpstr.size());
    this->strptr[tmpstr.size()] = '\0';
    return this->strptr;
}
simpid::SocketException::InvalidSocket::~InvalidSocket()
{
    delete this->strptr;
}




simpid::Socket::Socket(int domain, int type, 
                       int protocol)
{
#ifdef _WIN32
    WORD version_required = MAKEWORD(2, 2);
    WSADATA sock_msg;

    switch(WSAStartup(version_required, &sock_msg)) {
        case WSASYSNOTREADY:
            WSACleanup();
            if (IF_THROW) throw simpid::SocketException::SystemNotReady();
            break;
        case WSAVERNOTSUPPORTED:
            WSACleanup();
            if (IF_THROW) throw simpid::SocketException::VersionNotSupported();
            break;
        case WSAEPROCLIM:
            WSACleanup();
            if (IF_THROW) throw simpid::SocketException::TooManyProcesses();
            break;
        case WSAEINPROGRESS:
            WSACleanup();
            if (IF_THROW) throw simpid::SocketException::EventInProgress();
            break;
    }

    if (HIBYTE(sock_msg.wVersion) != 2
     || LOBYTE(sock_msg.wVersion) != 2) {
        WSACleanup();
        if (IF_THROW) throw simpid::SocketException::VersionNotExist();
    }
#endif

    this->skt = socket(domain, type, protocol);

#ifdef _WIN32
    if (this->skt == INVALID_SOCKET) {
        WSACleanup();
        if (IF_THROW) throw simpid::SocketException::InvalidSocket(WSAGetLastError());
    }
#else
    if (this->skt == -1) {
        fprintf(stderr, "errno: %d\n", errno);
        fprintf(stderr, "An error threw by socket(): %s\n", strerror(errno));
        exit(1);
    }
#endif
    this->recvbuf.resize(BUFSIZ);
    std::fill(this->recvbuf.begin(), this->recvbuf.end(), '\0');
}

simpid::Socket::~Socket()
{
#ifdef _WIN32
    closesocket(this->skt);
    WSACleanup();
#else
    close(this->skt);
#endif
}

int
simpid::Socket::bind(std::string ip, uint16_t port)
{
    int tmp;

#ifdef _WIN32
    SOCKADDR_IN servaddr;
#else
    struct sockaddr_in servaddr;
#endif
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    servaddr.sin_port = htons(port);

    tmp = 0;
    tmp = ::bind(this->skt, (struct sockaddr*)&servaddr, sizeof(servaddr));

#ifdef _WIN32
    if (tmp == SOCKET_ERROR) {
        closesocket(this->skt);
        WSACleanup();
        if (IF_THROW) throw simpid::SocketException::InvalidSocket(WSAGetLastError());
    }
#else
    if (tmp == -1) {
        close(this->skt);
        fprintf(stderr, "errno: %d\n", errno);
        fprintf(stderr, "An error threw by bind(): %s\n", strerror(errno));
        exit(2);
    }
#endif
    return tmp;
}

int
simpid::Socket::listen(int backlog)
{
    int tmp = 0;
    tmp = ::listen(this->skt, backlog);
#ifdef _WIN32
    if (tmp == SOCKET_ERROR) {
        closesocket(this->skt);
        WSACleanup();
        if (IF_THROW) throw simpid::SocketException::InvalidSocket(WSAGetLastError());
    }
#else
    if (tmp < 0) {
        close(this->skt);
        fprintf(stderr, "errno: %d\n", errno);
        fprintf(stderr, "An error threw by listen(): %s\n", strerror(errno));
        exit(3);
    }
#endif
    return tmp;
}

simpid::Client
simpid::Socket::accept()
{
#ifdef _WIN32
    SOCKADDR_IN cliaddr;
    int cliaddr_len;
#else
    sockaddr_in cliaddr;
    socklen_t cliaddr_len;
#endif
    Client cli;
    cli.skt = ::accept(this->skt, (struct sockaddr*)&cliaddr, &cliaddr_len);
#ifdef _WIN32
    if (cli.skt == INVALID_SOCKET) {
        closesocket(this->skt);
        WSACleanup();
        if (IF_THROW) throw simpid::SocketException::InvalidSocket(WSAGetLastError());
    }
#else
    if (cli.skt < 0) {
        ::close(this->skt);
        fprintf(stderr, "errno: %d\n", errno);
        fprintf(stderr, "An error threw by accept(): %s\n", strerror(errno));
        exit(4);
    }
#endif
    cli.ip = inet_ntoa(cliaddr.sin_addr);
    cli.port = cliaddr.sin_port;
    return cli;
}

int
simpid::Socket::connect(std::string ip, uint16_t port)
{
    int tmp;
#ifdef _WIN32
    SOCKADDR_IN servaddr;
#else
    struct sockaddr_in servaddr;
#endif
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    servaddr.sin_port = htons(port);

    tmp = 0;
    tmp = ::connect(this->skt, (struct sockaddr*)&servaddr, sizeof(servaddr));

#ifdef _WIN32
    if (tmp == SOCKET_ERROR) {
        closesocket(this->skt);
        WSACleanup();
        if (IF_THROW) throw simpid::SocketException::InvalidSocket(WSAGetLastError());
    }
#else
    if (tmp == -1) {
        ::close(this->skt);
        fprintf(stderr, "errno: %d\n", errno);
        fprintf(stderr, "An error threw by connect(): %s\n", strerror(errno));
        exit(5);
    }
#endif
    return tmp;
}

int
simpid::Socket::send(std::string buf)
{
    return this->send(buf.c_str(), buf.size());
}

int
simpid::Socket::send(const char *buf, size_t length)
{
    int tmp = 0;
    tmp = ::send(this->skt, buf, length, 0);
#ifdef _WIN32
    if (tmp == SOCKET_ERROR) {
        closesocket(this->skt);
        WSACleanup();
        if (IF_THROW) throw simpid::SocketException::InvalidSocket(WSAGetLastError());
    }
#else
    if (tmp < 0) {
        ::close(this->skt);
        fprintf(stderr, "errno: %d\n", errno);
        fprintf(stderr, "An error threw by connect(): %s\n", strerror(errno));
        exit(6);
    }
#endif
    return tmp;
}


std::string
simpid::Socket::recv(size_t length)
{
    this->recv(const_cast<char *>(this->recvbuf.c_str()), this->recvbuf.size());
    return this->recvbuf;
}

std::string
simpid::Socket::recvall()
{
    std::string msg;

    char tmp[BUFSIZ + 1];
    while (this->recv(tmp, BUFSIZ) == BUFSIZ) {
        msg = msg + (std::string)tmp;
        memset(tmp, 0, BUFSIZ + 1);
    }
    msg = msg + (std::string)tmp;
    memset(tmp, 0, BUFSIZ + 1);

    return msg;
}

int
simpid::Socket::recv(char *buf, size_t length)
{
    int tmp = 0;
    tmp = ::recv(this->skt, buf, length, 0);
    if (tmp >= 0) buf[tmp] = '\0';
#ifdef _WIN32
    if (tmp == SOCKET_ERROR) {
        closesocket(this->skt);
        WSACleanup();
        if (IF_THROW) throw simpid::SocketException::InvalidSocket(WSAGetLastError());
    }
#else
    if (tmp < 0) {
        close(this->skt);
        fprintf(stderr, "errno: %d\n", errno);
        fprintf(stderr, "An error threw by recv(): %s\n", strerror(errno));
        exit(7);
    }
#endif
    return tmp;
}

simpid::Client::Client()
{
    this->recvbuf.resize(BUFSIZ);
    std::fill(this->recvbuf.begin(), this->recvbuf.end(), '\0');
}

simpid::Client::~Client()
{
#ifdef _WIN32
    closesocket(this->skt);
#else
    close(this->skt);
#endif
}

int
simpid::Client::send(std::string buf)
{
    return this->send(buf.c_str(), buf.size());
}

int
simpid::Client::send(const char *buf, size_t length)
{
    int tmp = 0;
    tmp = ::send(this->skt, buf, length, 0);
#ifdef _WIN32
    if (tmp == SOCKET_ERROR) {
        closesocket(this->skt);
        if (IF_THROW) throw simpid::SocketException::InvalidSocket(WSAGetLastError());
    }
#else
    if (tmp < 0) {
        close(this->skt);
        fprintf(stderr, "errno: %d\n", errno);
        fprintf(stderr, "An error threw by connect(): %s\n", strerror(errno));
        exit(6);
    }
#endif
    return tmp;
}

std::string
simpid::Client::recv(size_t length)
{
    this->recv(const_cast<char *>(this->recvbuf.c_str()), this->recvbuf.size());
    return this->recvbuf;
}

std::string
simpid::Client::recvall()
{
    std::string msg;

    char tmp[BUFSIZ + 1];
    while (this->recv(tmp, BUFSIZ) == BUFSIZ) {
        msg = msg + (std::string)tmp;
        memset(tmp, 0, BUFSIZ + 1);
    }
    msg = msg + (std::string)tmp;
    memset(tmp, 0, BUFSIZ + 1);

    return msg;
}

int
simpid::Client::recv(char *buf, size_t length)
{
    int tmp = 0;
    tmp = ::recv(this->skt, buf, length, 0);
    if (tmp >= 0) buf[tmp] = '\0';
#ifdef _WIN32       // Windows
    if (tmp == SOCKET_ERROR) {
        closesocket(this->skt);
        if (IF_THROW) throw simpid::SocketException::InvalidSocket(WSAGetLastError());
    }
#else               // Unix
    if (tmp < 0) {
        close(this->skt);
        fprintf(stderr, "errno: %d\n", errno);
        fprintf(stderr, "An error threw by recv(): %s\n", strerror(errno));
        exit(7);
    }
#endif
    return tmp;
}
