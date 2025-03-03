#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <deque>
#include <thread>

using namespace sf;
using namespace std;

std::deque<Packet *> packet_queue;
//sudo lsof -i -P -n | grep LISTEN
const unsigned short port = 9774;

class Player {
public:
    int id;
    string uname;
    long data{};
    Vector2f pos;
    int region;

    // placeholder function (should eventually return actionable data)
    long blob() {
        data += 1;
        cout << "Player " << id << " data: " << data << endl;
        return 0;
    }

    long deblob() {
        data -= 1;
        return 0;
    }

    explicit Player(const string &uname) {
        id = next_id();
        Player::uname = uname;
    }

private:
    static int next_id() {
        return 0;
    }
};


void queue_packets() {
    cout << "Packet thread is running!" << endl;
    UdpSocket socket;
    if (socket.bind(port) != Socket::Status::Done)
        cout << "Error" << endl;
    while (true) {
        auto *packet = new Packet();
        optional<IpAddress> sender;
        unsigned short port;
        if (socket.receive(*packet, sender, port) != Socket::Status::Done) {
            // error...
        }
        int testvar;
        *packet >> testvar;
        if (testvar == 2) {
            cout << "DEBUG: Received data from " << sender->toString() << " on port " << port << std::endl;
            packet_queue.push_back(packet);
        }


    }
}

void process_packets(Player *player) {
    while (true) {
        if (!packet_queue.empty()) {
            // process packets
        }
    }
}


int main() {
    const unsigned window_width = VideoMode::getDesktopMode().size.x / 2;
    const unsigned window_height = VideoMode::getDesktopMode().size.y;
    RenderWindow window(sf::VideoMode({window_width, window_height}), "Client");
    window.setPosition({static_cast<int>(window_width), 0});
    window.setFramerateLimit(60); // call it once after creating the window

    Vector2f pos = {380, 380};
    Texture texture("assets/jared.png");
    Sprite p_sprite(texture);
    p_sprite.scale({0.5, 0.5});
    p_sprite.setPosition(pos);
    p_sprite.setOrigin({static_cast<float>(p_sprite.getTexture().getSize().x/2), static_cast<float>(p_sprite.getTexture().getSize().y/2)});

    bool isMovingRight;
    bool isMovingLeft;
    bool isMovingUp;
    bool isMovingDown;

    Player player("Davis");



    UdpSocket socket;
    IpAddress recipient = IpAddress(IpAddress::getLocalAddress().value());
    cout << "IP Address: " << recipient.toString() << endl;
    Clock clock;
    int speed = 200;

    std::thread receiver(queue_packets);
    receiver.detach();
    std::thread processor(process_packets, &player);
    processor.detach();
    // eventually should be triggered by initialization packets
    while (window.isOpen()) {
        // set timedelta
        float delta = clock.restart().asSeconds();

        // Check all window events triggered this run-through of the loop`
        while (const optional<Event> event = window.pollEvent()) {

            // "close requested" event: we close the window
            if (event->is<Event::Closed>())
                window.close();
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if ((keyPressed->scancode == Keyboard::Scancode::Right) or (keyPressed->scancode == Keyboard::Scancode::D)) {
                    isMovingRight = true;
                }
                if ((keyPressed->scancode == Keyboard::Scancode::Left) or (keyPressed->scancode == Keyboard::Scancode::A)) {
                    isMovingLeft = true;
                }
                if ((keyPressed->scancode == Keyboard::Scancode::Up) or (keyPressed->scancode == Keyboard::Scancode::W)) {
                    isMovingUp = true;
                }
                if ((keyPressed->scancode == Keyboard::Scancode::Down) or (keyPressed->scancode == Keyboard::Scancode::S)) {
                    isMovingDown = true;
                }
            }
            if (const auto* keyReleased = event->getIf<Event::KeyReleased>()) {
                if ((keyReleased->scancode == Keyboard::Scancode::Right) or (keyReleased->scancode == Keyboard::Scancode::D)) {
                    isMovingRight = false;
                }
                if ((keyReleased->scancode == Keyboard::Scancode::Left) or (keyReleased->scancode == Keyboard::Scancode::A)) {
                    isMovingLeft = false;
                }
                if ((keyReleased->scancode == Keyboard::Scancode::Up) or (keyReleased->scancode == Keyboard::Scancode::W)) {
                    isMovingUp = false;
                }
                if ((keyReleased->scancode == Keyboard::Scancode::Down) or (keyReleased->scancode == Keyboard::Scancode::S)) {
                    isMovingDown = false;
                }
            }
        }


        if (isMovingRight) {
            pos.x += speed*delta;
        }
        if (isMovingLeft) {
            pos.x -= speed*delta;
        }
        if (isMovingUp) {
            pos.y -= speed*delta;
        }
        if (isMovingDown) {
            pos.y += speed*delta;
        }

        Packet p;
        p << 1 << 0 << pos.x << pos.y;
        if (socket.send(p, recipient, port) != Socket::Status::Done)
        {
            std::cout << "didn't send socket for some reason" << std::endl;
        }


        // clear the window with black color
        window.clear(sf::Color::Black);
        p_sprite.setPosition(pos);
        // DRAW EVERYTHING HERE
        window.draw(p_sprite);

        window.display();
    }

    return 0;
}
