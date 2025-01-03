#ifndef SERVER_HH
#define SERVER_HH

#include "../socket/socket.hh"
#include "packets.hh"
#include <string>
#include <deque>
#include <vector>


namespace srv{

    class Server{
        public:
            Server(std::string ip, uint16_t port, Packet* (*pack_up_func)(char*,size_t));
            virtual ~Server();
            void run();

        private:
            sck::Socket socket;
            Packet* (*pack_up_func)(char*, size_t) = nullptr;
            std::vector<sck::Socket> connections{};
            std::deque<Packet*> handle_buffer{};
            std::deque<Packet*> send_buffer{};

    };
}

#endif