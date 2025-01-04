#include "socket_linux.hh"
#include <system_error>
#include <arpa/inet.h>
#include <inttypes.h>
#include <utility>

namespace sck
{
#include <unistd.h>

    Socket::Socket(ADDRESS_FAMILY const af, SOCKET_TYPE const type, SOCKET_PROTOCOL const protocol) : sock{socket(af, type, protocol)}, fam{af}, type{type}, protocol{protocol}
    {
        if (sock == -1)
        {
            sock = 0;
            throw std::system_error(std::error_code(errno, std::system_category()), "Error creating socket");
        }
    }

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

    void Socket::bind(std::string address, uint16_t port) const
    {   
        int inet_len = (fam == sck::INET) ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN;

        sockaddr_storage addr_converted{};

        int addr_len{};
        int succ{inet_pton(fam,address.data(), &addr_converted)};

        if (succ != 0){
            throw std::system_error(std::error_code(errno, std::system_category()), "Error tying to bind socket to address");
        }


        if (fam == sck::INET){
            addr_converted->sin_port = htons(port);

        }

        succ = sck::bind(sock, &addr_converted, addr_len);
    }

    void Socket::listen(unsigned int const queue) const
    {
        sck::listen(sock, queue);
    }

    Socket Socket::accept()
    {   
        sockaddr temp{};
        unsigned int addr_len{};
        
        sck::SOCKET conn{sck::accept(sock,&temp,&addr_len)};
        Socket conn_soc{this->fam, this->type, this->protocol};
        conn_soc.sock = conn;
        bound_to = static_cast<sockaddr_in>(temp);
        sockaddr_size = addr_len;
        return conn_soc;
    }

    int Socket::close()
    {   
        if (sock != 0){
            int val{sck::close(sock)};
            if (val == 0)
                sock = 0;
        }
    }

    int Socket::send(char *buffer, unsigned int const size) const
    {
        return sck::send(sock,buffer, size,0);
    }

    int Socket::recv(char *buffer, unsigned int const size, unsigned int const len) const
    {
        return sck::recv(sock, buffer, size, 0);
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
