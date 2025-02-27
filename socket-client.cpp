#include <iostream>
#include <SFML/Network.hpp>

using namespace sf;

int main() {
    UdpSocket socket;

    Packet packet;
    packet << "hello";
    // UDP socket:
    IpAddress recipient(192, 168, 1, 153);
    const unsigned short port = 54000;
    if (socket.send(packet, recipient, port) != sf::Socket::Status::Done)
    {
        std::cout << "didn't send socket for some reason" << std::endl;
    }
    return 0;
}