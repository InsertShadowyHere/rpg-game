#include <SFML/Network.hpp>
#include <iostream>
#include <string>

using namespace sf;
int main() {
    UdpSocket socket;

    // bind the socket to a port
    if (socket.bind(54000) != Socket::Status::Done)
    {
        std::cout << "Oh no! The socket didn't bind!" << std::endl;
    }
    
    IpAddress sender();
    unsigned short port;
    if (socket.receive(data.data(), data.size(), received, sender, port) != sf::Socket::Status::Done)
    {
        // error...
    }
    std::cout << "Received " << received << " bytes from " << sender << " on port " << port << std::endl;

    return 0;
}