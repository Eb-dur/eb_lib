#include "socket.hh"

#include <utility>
#include <system_error>
#include <winsock.h>
#include <WS2tcpip.h>

using namespace sck;

    Socket::Socket(INET_TYPE const af, SOCKET_TYPE const type, SOCKET_PROTOCOL const protocol) : sock{socket(af, type, protocol)}, fam{af}, type{type}, protocol{protocol}
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

    Socket::Socket(Socket && other) : sock{::std::exchange(other.sock, 0)}, 
    fam{::std::exchange(other.fam, UNSPEC)},
    type{::std::exchange(other.type, NONE)},
    protocol{::std::exchange(other.protocol, DEFAULT)}
    {

    }

    Socket &Socket::operator=(Socket && other)
    {
        // Empty definition
        if (this != &other){
            close();
            sock = ::std::exchange(other.sock, 0);
            fam = ::std::exchange(other.fam, INET_TYPE::UNSPEC);
            type = ::std::exchange(other.type, SOCKET_TYPE::NONE);
            protocol = ::std::exchange(other.protocol, SOCKET_PROTOCOL::DEFAULT);
        }
        return *this;
    }

    Socket::~Socket()
    {   
        this->close();
        delete bound_to;
    }

    void Socket::bind(std::string address, uint16_t port)
    {   
        sockaddr_storage* addr_converted{new sockaddr_storage{}};
        this->bound_to = addr_converted;
        int addr_len{};
        int succ{WSAStringToAddressA(address.data(), fam, NULL, reinterpret_cast<sockaddr*>(addr_converted), &addr_len)};
        if (succ != 0){
            throw std::system_error(std::error_code(WSAGetLastError(), std::system_category()), "Error tying to bind socket to address");
        }
        
        if (fam == ::INET_TYPE::INET6){
            sockaddr_in6* addr{reinterpret_cast<sockaddr_in6*>(addr_converted)};
            addr->sin6_port = port;
            succ = ::bind(sock, reinterpret_cast<sockaddr*>(addr), addr_len);
        }
        else if ( fam == sck::INET_TYPE::INET){
            sockaddr_in* addr{reinterpret_cast<sockaddr_in*>(addr_converted)};
            addr->sin_port = port;
            succ = ::bind(sock, reinterpret_cast<sockaddr*>(addr), addr_len);
        }

    }

    int Socket::connect(std::string address, uint16_t port)
    {   
        sockaddr_storage* addr_converted{new sockaddr_storage{}};
        
        int addr_len{};
        int succ{WSAStringToAddressA(address.data(), fam, NULL, reinterpret_cast<sockaddr*>(addr_converted), &addr_len)};
        if (succ != 0){
            throw std::system_error(std::error_code(WSAGetLastError(), std::system_category()), "Error tying to bind socket to address");
        }
        
        if (fam == INET_TYPE::INET6){
            sockaddr_in6* addr{reinterpret_cast<sockaddr_in6*>(addr_converted)};
            addr->sin6_port = port;
            succ = ::connect(sock, reinterpret_cast<sockaddr*>(addr), addr_len);
        }
        else if ( fam == INET_TYPE::INET){
            sockaddr_in* addr{reinterpret_cast<sockaddr_in*>(addr_converted)};
            addr->sin_port = port;
            succ = ::connect(sock, reinterpret_cast<sockaddr*>(addr), addr_len);
        }
        
        this->bound_to = addr_converted;
        return succ;
    }

    void Socket::listen(unsigned int const queue) const
    {
        ::listen(sock, queue);
    }

    Socket Socket::accept()
    {   
        sockaddr_storage* addr{new sockaddr_storage{}};
        int addr_len{};
        
        sck::SOCKET conn{::accept(sock,reinterpret_cast<sockaddr*>(addr),&addr_len)};
        
        if (conn == INVALID_SOCKET) {
        throw std::system_error(std::error_code(WSAGetLastError(), std::system_category()), "Error accepting connection");
    }

        Socket conn_soc{this->fam, this->type, this->protocol};

        conn_soc.sock = conn;
        conn_soc.bound_to = addr;
        conn_soc.sockaddr_size = addr_len;
        return conn_soc;
    }

    int Socket::close()
    {   
        if (sock != 0){
            int val{closesocket(sock)};
            if (val == 0)
                sock = 0;
        }
        return sock;
    }

    int Socket::send(char *buffer, unsigned int const size) const
    {
        return ::send(sock,buffer, size,0);
    }

    int Socket::recv(char *buffer, unsigned int const size) const
    {
        return ::recv(sock, buffer, size, 0);
    }

    bool Socket::operator==(Socket &other) const
    {
        return sock == other.sock && fam == other.fam && type == other.type && protocol == other.protocol && sockaddr_size == other.sockaddr_size;
    }

    bool Socket::operator!=(Socket &other) const
    {
        return !(*this==other);
    }

