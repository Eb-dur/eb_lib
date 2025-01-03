#include <iostream>
#include "packets.TT"



int main(){
    srv::Packet<1> a;
    std::cout << *((unsigned int*) a.data()) << std::endl;
}