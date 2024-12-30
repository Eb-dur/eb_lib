#include "socket_win32.hh"

namespace sck {

Socket::Socket(ADDRESS_FAMILY const af, SOCKET_TYPE const typ, SOCKET_PROTOCOL const protocol) {
    // Empty definition
}

Socket::Socket(Socket& const other) {
    // Empty definition
}

Socket& Socket::operator=(Socket& const other) {
    // Empty definition
    return *this;
}

Socket::Socket(Socket&& const other) {
    // Empty definition
}

Socket& Socket::operator=(Socket&& const other) {
    // Empty definition
    return *this;
}

Socket::~Socket() {
    // Empty definition
}

void Socket::bind(std::string address) {
    // Empty definition
}

void Socket::listen() {
    // Empty definition
}

void Socket::accept() {
    // Empty definition
}

void Socket::close() {
    // Empty definition
}

unsigned int Socket::send(char** buffer, unsigned int const size) const {
    // Empty definition
    return 0;
}

unsigned int Socket::recieve(char** buffer, unsigned int const size, unsigned int const len) const {
    // Empty definition
    return 0;
}

} // namespace sck
