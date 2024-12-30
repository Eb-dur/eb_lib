#include <winsock2.h>
#include <string>

namespace sck
{
    
enum ADDRESS_FAMILY{
    UNSPEC = 0,
    INET = 2,
    IPX = 6,
    APPLETALK = 16,
    NETBIOS = 17,
    INET6 = 23,
    IRDA = 26,
    BTH = 32
};

enum class SOCKET_TYPE{
    STREAM = 1,
    DGRAM = 2,
    RAW = 3,
    RDM = 4,
    SEQPACKET = 5,
};

enum SOCKET_PROTOCOL{
    DEFAULT = 0,
    IPPROTO_ICMP = 1,
    IPPROTO_IGMP = 2,
    BTHPROTO_RFCOMM = 3,
    IPPROTO_TCP = 6,
    IPPROTO_UDP = 17,
    IPPROTO_ICMPV6 = 58,
    IPPROTO_RM = 113,
};

class Socket{
    public:
        Socket(ADDRESS_FAMILY const af, 
        SOCKET_TYPE const typ, 
        SOCKET_PROTOCOL const protocol = SOCKET_PROTOCOL::DEFAULT
        );
        Socket(Socket& const other);
        Socket& operator=(Socket& const other);
        Socket(Socket&& const other);
        Socket& operator=(Socket&& const other);

        ~Socket();
        void bind(std::string address);
        void listen();
        void accept();
        void close();
        unsigned int send(char** buffer, unsigned int const size) const;
        unsigned int recieve(char** buffer, unsigned int const size, unsigned int const len) const;

    private:
        SOCKET sock{};



};

}