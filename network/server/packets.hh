#ifndef PACKETS_HH
#define PACKETS_HH
#include "../socket/socket.hh"

/*

    All packets used in the same program should have a different id
    so that the server can deduct which it is.


*/

namespace srv{

    class Packet
    {
        public:
            virtual ~Packet() = default;
            virtual void initialize(char* bytes) = 0;
            virtual int send(sck::Socket s) = 0;
            virtual unsigned char id() = 0;
        protected:
            unsigned int sender{};
            unsigned int receiver{};
    };


}

#endif