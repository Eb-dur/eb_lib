#ifndef SOCKET_LINUX_HH
#define SOCKET_LINUX_HH
#include <string>

namespace sck
{   
    using SOCKET = int;
    #include <sys/socket.h>

enum ADDRESS_FAMILY{
    UNSPEC = AF_UNSPEC,
    INET = AF_INET,
    INET6 = AF_INET6,
};

enum SOCKET_TYPE{
    NONE = 0,
    STREAM = SOCK_STREAM,
    DGRAM = SOCK_DGRAM,
    RAW = SOCK_RAW,
    RDM = SOCK_RDM,
    SEQPACKET = SOCK_SEQPACKET,
};

enum SOCKET_PROTOCOL{
    DEFAULT = 0,
    ICMP = 1,
    IGMP = 2,
    TCP = 6,
    UDP = 17,
    ICMPV6 = 58,
};

class Socket{
    public:
        Socket(ADDRESS_FAMILY const af, 
        SOCKET_TYPE const typ, 
        SOCKET_PROTOCOL const protocol = SOCKET_PROTOCOL::DEFAULT
        );
        Socket(Socket& const other) = delete;
        Socket& operator=(Socket& const other) = delete;
        Socket(Socket&& other);
        Socket& operator=(Socket&& other);

        bool operator==(Socket& other) const;
        bool operator!=(Socket& other) const;

        ~Socket();
        void bind(std::string address, uint16_t const port) const;
        void listen(unsigned int const queue) const;
        Socket accept();
        int close();
        int send(char* buffer, unsigned int const size) const;
        int recv(char* buffer, unsigned int const size, unsigned int const len) const;

    private:
        // TODO: Add free of this in destructor
        sockaddr_storage* bound_to{};
        SOCKET sock{};
        ADDRESS_FAMILY fam{};
        SOCKET_TYPE type{};
        SOCKET_PROTOCOL protocol{};
        unsigned int sockaddr_size{};



};

}

#endif