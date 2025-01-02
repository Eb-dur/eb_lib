
#include <string>

namespace sck
{
    #include <winsock2.h>

enum ADDRESS_FAMILY{
    UNSPEC = 0,
    INET = 2,
    INET6 = 23,
};

enum SOCKET_TYPE{
    NONE = 0,
    STREAM = 1,
    DGRAM = 2,
    RAW = 3,
    RDM = 4,
    SEQPACKET = 5,
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
        void bind(std::string address) const;
        void listen(unsigned int const queue) const;
        Socket accept();
        int close();
        int send(char* buffer, unsigned int const size) const;
        int recv(char* buffer, unsigned int const size, unsigned int const len) const;

    private:
        SOCKET sock{};
        ADDRESS_FAMILY fam{};
        SOCKET_TYPE type{};
        SOCKET_PROTOCOL protocol{};
        sockaddr bound_to{};
        int sockaddr_size{};



};

}