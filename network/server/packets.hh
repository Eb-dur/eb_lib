#ifndef PACKETS_HH
#define PACKETS_HH
#include "../socket/socket.hh"

namespace srv{

    class Packet
    {
        public:
            virtual ~Packet() = default;
            
            virtual void initialize(char* bytes) = 0;
            virtual int send(sck::Socket s) = 0;
        protected:
            unsigned int sender{};
            unsigned int receiver{};
    };
    
}

#endif