#ifndef SOCKET_HH
#define SOCKET_HH

#include <string>

namespace sck {

#ifdef _WIN32
    #include <winsock2.h>
    using SOCKET = SOCKET;
    enum ADDRESS_FAMILY {
        UNSPEC = 0,
        INET = 2,
        INET6 = 23,
    };
    enum SOCKET_TYPE {
        NONE = 0,
        STREAM = 1,
        DGRAM = 2,
        RAW = 3,
        RDM = 4,
        SEQPACKET = 5,
    };
    enum SOCKET_PROTOCOL {
        DEFAULT = 0,
        ICMP = 1,
        IGMP = 2,
        TCP = 6,
        UDP = 17,
        ICMPV6 = 58,
    };
// If not windows it is linux
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    using SOCKET = int;
    enum ADDRESS_FAMILY {
        UNSPEC = AF_UNSPEC,
        INET = AF_INET,
        INET6 = AF_INET6,
    };
    enum SOCKET_TYPE {
        NONE = 0,
        STREAM = SOCK_STREAM,
        DGRAM = SOCK_DGRAM,
        RAW = SOCK_RAW,
        RDM = SOCK_RDM,
        SEQPACKET = SOCK_SEQPACKET,
    };
    enum SOCKET_PROTOCOL {
        DEFAULT = 0,
        ICMP = 1,
        IGMP = 2,
        TCP = 6,
        UDP = 17,
        ICMPV6 = 58,
    };
#endif

class Socket {
public:
    Socket(ADDRESS_FAMILY const af, SOCKET_TYPE const type, SOCKET_PROTOCOL const protocol = SOCKET_PROTOCOL::DEFAULT);
    Socket(Socket& const other) = delete;
    Socket& operator=(Socket& const other) = delete;
    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    bool operator==(Socket& other) const;
    bool operator!=(Socket& other) const;

    ~Socket();
    void bind(std::string address, uint16_t port = 0) const;
    void listen(unsigned int const queue) const;
    Socket accept();
    int close();
    int send(char* buffer, unsigned int const size) const;
    int recv(char* buffer, unsigned int const size, unsigned int const len) const;

private:
    sockaddr_storage* bound_to{};
    SOCKET sock{};
    ADDRESS_FAMILY fam{};
    SOCKET_TYPE type{};
    SOCKET_PROTOCOL protocol{};
    unsigned int sockaddr_size{};
};

}
#endif