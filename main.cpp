#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <queue>
#include <random>
#include <thread>

using namespace sf;

std::deque<Packet *> packet_queue;

void queue_packets() {
    std::cout << "Packet thread is running!" << std::endl;
    UdpSocket socket;
    if (socket.bind(54000) != sf::Socket::Status::Done)
        std::cout << "Error" << std::endl;
    while (true) {
        auto *packet = new Packet();
        std::optional<IpAddress> sender;
        unsigned short port;
        if (socket.receive(*packet, sender, port) != Socket::Status::Done) {
            // error...
        }
        std::cout << "DEBUG: Received data from " << sender->toString() << " on port " << port << std::endl;
        packet_queue.push_back(packet);
    }
}

using namespace std;

/// Handles player data, blobbing, and such
///
/// @param id int
/// @param uname string
class Player {
public:
    int id = next_id();
    string uname;
    long data = 1;
    int x = 0, y = 0;

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

class Game {
public:
    vector<Player *> players;

    // Fetch player data, add player to vector
    void add_player(const string &uname) {
        cout << "DEBUG: Adding player " << uname << endl;
        auto *player = new Player(uname);
        players.push_back(player);
    }

    void print_players() {
        std::cout << "DEBUG: Printing players" << std::endl;
        for (const Player *i: players) {
            std::cout << "    Player " << i->id << ": " << players[0]->uname << std::endl;
        }
    }

    void process(Packet *packet) {
        /* Packet types
         * 8 - initialization - take char data and
         */
        cout << "START PACKET PROCESSING" << endl;
        int type;
        *packet >> type;
        cout << "   Packet type: " << type << endl;
        switch (type) {
            case 0: {
                // Initialization packet
                string uname;
                *packet >> uname;
                add_player(uname);
                break;
            }
            case 1: {
                // PosUpdate
                int pid;
                *packet >> pid;
                for (Player *i: players)
                    if (i->id == pid) {
                        *packet >> i->x >> i->y;
                        cout << "   Player " << i->id << " position updated." << endl;
                        break;
                    }
                break;
            }
            default: {
                cout << "   ALERT! Packet " << packet << "type unknown" << endl;
                break;
            }
        }
    }
};


int main() {
    // texture initialization (eventually change to region struct)
    // eventually should be handled through initialization/region-swap packets
    Texture bg_texture("assets/bg.png");
    Sprite bg_sprite(bg_texture);

    // Main game object
    Game game;
    game.add_player("Davis");
    game.print_players();

    // create the window and initialize some things
    RenderWindow window(sf::VideoMode({800, 600}), "My window");
    window.setFramerateLimit(60); // call it once after creating the window

    // test image
    Texture texture("assets/jared.png");
    Sprite p_sprite(texture);
    p_sprite.scale({0.5, 0.5});
    p_sprite.setPosition({0, 380});

    p_sprite.setOrigin({static_cast<float>(p_sprite.getTexture().getSize().x/2), static_cast<float>(p_sprite.getTexture().getSize().y/2)});

    /* Main loop; run program while window is open.
     * Check for events
     * Take server data
     * Send client data
     * update things that need to be updated
     * Render background
     * Render entities */
    std::thread receiver(queue_packets);
    receiver.detach();
    // eventually should be triggered by initialization packets
    while (window.isOpen()) {
        // Processes packets
        if (!packet_queue.empty()) {
            game.process(packet_queue.front());
            packet_queue.pop_front();
        }

        // Check all window events triggered this run-through of the loop
        while (const std::optional event = window.pollEvent()) {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        if (isKeyPressed(sf::Keyboard::Scan::Right)) {
            cout << "hello" << endl;
        }

        // Handle key presses only when in focus
        if (window.hasFocus()) {
            p_sprite.setPosition(Vector2f(Mouse::getPosition(window)));
            // if the left button is down, give bg a lil push
            if (isButtonPressed(sf::Mouse::Button::Left)) {
                // Character moves left, screen moves right
                if (sf::Mouse::getPosition(window).x < window.getSize().x / 2) {
                    bg_sprite.move({5, 0});
                }
                // character moves right, screen moves left
                else
                    bg_sprite.move({-5, 0});
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);


        // DRAW EVERYTHING HERE
        window.draw(bg_sprite);
        // window.draw(...);
        window.draw(p_sprite);

        // if (!game.players.empty()) {
        //     for (Player *i : game.players) {
        //         break;
        //     }
        //
        // }

        // end the current frame
        window.display();
    }
    return 0;
}
