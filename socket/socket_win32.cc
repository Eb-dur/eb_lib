#include "socket_win32.hh"
#include <system_error>
#include <winsock.h>

namespace sck
{

    Socket::Socket(ADDRESS_FAMILY const af, SOCKET_TYPE const type, SOCKET_PROTOCOL const protocol) : sock{socket(af, type, protocol)}, fam{af}, type{type}, protocol{protocol}
    {
        if (sock == INVALID_SOCKET)
        {
            sock = 0;
            throw std::system_error(std::error_code(WSAGetLastError(), std::system_category()), "Error creating socket");
        }
    }
    /*
    Socket::Socket(Socket &const other) : sock{other.sock}
    {
    }

    Socket &Socket::operator=(Socket &const other)
    {
        if (*this == other)
            return *this;

        sock = other.sock;
        fam = other.fam;
        type = other.type;
        protocol = other.protocol;

        return *this;
    }
    */

    Socket::Socket(Socket && other) : sock{std::exchange(other.sock, 0)}, 
    fam{std::exchange(other.fam, 0)},
    type{std::exchange(other.type, 0)}, 
    protocol{std::exchange(other.protocol, 0)}
    {

    }

    Socket &Socket::operator=(Socket && other)
    {
        // Empty definition
        if (this != &other){
            close();
            sock = std::exchange(other.sock, 0);
            fam = std::exchange(other.fam, ADDRESS_FAMILY::UNSPEC);
            type = std::exchange(other.type, SOCKET_TYPE::NONE);
            protocol = std::exchange(other.protocol, SOCKET_PROTOCOL::DEFAULT);
        }
        return *this;
    }

    Socket::~Socket()
    {   
        this->close();
    }

    void Socket::bind(std::string address) const
    {   
        sockaddr addr_converted{};
        int addr_len{};
        int succ{WSAStringToAddressA(address.data(), fam, NULL, &addr_converted, &addr_len)};
        if (succ != 0){
            throw std::system_error(std::error_code(WSAGetLastError(), std::system_category()), "Error tying to bind socket to address");
        }
        succ = sck::bind(sock, &addr_converted, addr_len);
    }

    void Socket::listen(unsigned int const queue) const
    {
        sck::listen(sock, queue);
    }

    Socket Socket::accept()
    {   
        sck::sockaddr temp{};
        int addr_len{};
        
        sck::SOCKET conn{sck::accept(sock,&temp,&addr_len)};
        Socket conn_soc{this->fam, this->type, this->protocol};
        conn_soc.sock = conn;
        bound_to = temp;
        sockaddr_size = addr_len;
        return conn_soc;
    }

    int Socket::close()
    {   
        if (sock != 0){
            int val{sck::closesocket(sock)};
            if (val == 0)
                sock = 0;
        }
    }

    unsigned int Socket::send(char **buffer, unsigned int const size) const
    {
        // Empty definition
        return 0;
    }

    unsigned int Socket::recv(char **buffer, unsigned int const size, unsigned int const len) const
    {
        // Empty definition
        return 0;
    }

    bool Socket::operator==(Socket &other) const
    {
        return sock == other.sock && fam == other.fam && type == other.type && protocol == other.protocol && sockaddr_size == other.sockaddr_size;
    }

    bool Socket::operator!=(Socket &other) const
    {
        return !(*this==other);
    }

}
