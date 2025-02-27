#include <iostream>
#include <SFML/Network.hpp>

using namespace sf;

int main() {
    UdpSocket socket;
    if (socket.bind(54000) != sf::Socket::Status::Done)
    {
        std::cout << "Error" << std::endl;
    }

    Packet packet;
    std::optional<IpAddress> sender;
    unsigned short port;
    if (socket.receive(packet, sender, port) != sf::Socket::Status::Done)
    {
        // error...
    }
    std::cout << "Received " << packet.getData() << " from " << sender->toString() << " on port " << port << std::endl;
    return 0;
}