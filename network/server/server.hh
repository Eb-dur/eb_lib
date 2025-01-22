#ifndef SERVER_HH
#define SERVER_HH

#include "../socket/socket.hh"
#include "packets.hh"
#include <string>
#include <deque>
#include <vector>

/*

    The pack_up_func should be a function that can handle all sorts of
    packages that the server should be able to handle. 

*/

namespace srv{

    class Server{
        public:
            Server(std::string ip, uint16_t port, Packet* (*pack_up_func)(char*,size_t));
            virtual ~Server();
            void run();

        private:
            sck::Socket socket;
            Packet* (*pack_up_func)(char*, size_t){nullptr};
            std::vector<sck::Socket> connections{};
            std::deque<Packet*> handle_buffer{};
            std::deque<Packet*> send_buffer{};

    };
}

#endif