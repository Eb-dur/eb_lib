#include <iostream>
#include "socket.hh"




int main(){
    sck::Socket sock{sck::INET, sck::STREAM};
    sock.bind("127.0.0.1", 4040);
    sock.listen(1);
    sock.accept();
    std::cout << "Socket accepted" << std::endl;
    char buffer[256];
    int read = sock.recv(buffer, 256);
    std::string s{buffer,buffer+read};
    std::cout << "Read theese bytes: " << s << std::endl;
    sock.close();
}