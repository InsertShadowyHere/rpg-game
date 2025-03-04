#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <queue>
#include <random>
#include <thread>
#include <nlohmann/json.hpp>
#include <fstream>

using namespace sf;
using namespace std;
using namespace nlohmann;

std::deque<Packet *> packet_queue;
//sudo lsof -i -P -n | grep LISTEN
const unsigned short port = 567;

void queue_packets(UdpSocket *socket) {
    cout << "DEBUG: Queuing packets!" << endl;
    if (socket->bind(port) != Socket::Status::Done)
        cout << "Socket bind failed!" << endl;
    while (true) {
        auto *packet = new Packet();
        optional<IpAddress> sender;
        unsigned short temp_port;
        if (socket->receive(*packet, sender, temp_port) != Socket::Status::Done) {
            // error...
        }
        std::cout << "DEBUG: Received data from " << sender->toString() << " on port " << temp_port << std::endl;
        packet_queue.push_back(packet);
    }
}


/// Handles player data, blobbing, and such
///
/// @param id int
/// @param uname string
class Player {
public:
    int id;
    string uname;
    long data;
    Vector2f pos;
    int region;
    string texture_path;
    Sprite self;
    Texture texture;

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

    explicit Player(const string &username): self(texture) {
        id = next_id();
        uname = username;
        ifstream f("player-data.json");
        json data = json::parse(f)[uname];
        pos.x = data["pos-x"];
        pos.y = data["pos-y"];
        region = data["region"];
        texture_path = data["sprite-path"];
        cout << "DEBUG: Initializing player " << username << endl;
        texture = Texture(texture_path);
        self = Sprite(texture);
        self.setScale({0.5, 0.5});
        self.setOrigin({static_cast<float>(texture.getSize().x/2), static_cast<float>(texture.getSize().y/2)});
    }



private:
    static int next_id() {
        return 0;
    }
};
Packet& operator<<(Packet& packet, const Player& player)
{
    return packet << player.id << player.uname << player.pos.x << player.pos.y << player.region << player.texture_path;
}

Packet& operator>>(sf::Packet& packet, Player& player)
{
    return packet >> player.id >> player.uname >> player.pos.x >> player.pos.y >> player.region >> player.texture_path;
}

class BoundingBox {
public:
    Texture texture;
    Sprite sprite;
    string event;
};


class Region {
public:
    Texture background;
    Sprite bg_sprite;
    vector<BoundingBox> bounding_boxes;
};

class Game {
public:
    vector<Player *> players;

    // Fetch player data, add player to vector
    void add_player(Player *player) {
        cout << "DEBUG: Adding player " << player->uname << endl;
        players.push_back(player);
    }

    void print_players() {
        std::cout << "DEBUG: Printing players" << std::endl;
        for (const Player *i: players) {
            std::cout << "    Player " << i->id << ": " << players[0]->uname << std::endl;
        }
    }

    void process(Packet *packet, UdpSocket *socket) {
        cout << "START PACKET PROCESSING" << endl;
        int type;
        *packet >> type;
        cout << "   Packet type: " << type << endl;
        switch (type) {
            case 0: {
                // Initialization packet
                string uname;
                *packet >> uname;
                Player p(uname);
                Packet init_packet;
                init_packet << p;
                IpAddress recipient = IpAddress(IpAddress::getLocalAddress().value());
                if (socket->send(init_packet, recipient, port) != Socket::Status::Done) {
                    cout << "Couldn't confirm player initialization." << endl;
                }
                cout << "Confirmed palyer initialization" << endl;
                add_player(&p);
                break;
            }
            case 1: {
                // PosUpdate
                int pid;
                *packet >> pid;
                for (Player *i: players)
                    if (i->id == pid) {
                        *packet >> i->pos.x >> i->pos.y;
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
    UdpSocket socket;
    Texture bg_texture("assets/bg.png");
    Sprite bg_sprite(bg_texture);

    // Main game object
    Game game;


    // create the window and initialize some things
    const unsigned window_width = VideoMode::getDesktopMode().size.x / 2;
    const unsigned window_height = VideoMode::getDesktopMode().size.y;
    RenderWindow window(VideoMode({window_width, window_height}), "Server");
    window.setPosition({0,0});
    window.setFramerateLimit(60);

    // test image
    /* Main loop; run program while window is open.
     * Check for events
     * Take server data
     * Send client data
     * update things that need to be updated
     * Render background
     * Render entities */
    std::thread receiver(queue_packets, &socket);
    receiver.detach();

    // eventually should be triggered by initialization packets
    while (window.isOpen()) {
        // Processes packets
        if (!packet_queue.empty()) {
            game.process(packet_queue.front(), &socket);
            packet_queue.pop_front();
        }


        // Check all window events triggered this run-through of the loop
        while (const std::optional event = window.pollEvent()) {
            // "close requested" event: we close the window
            if (event->is<Event::Closed>())
                window.close();

        }




        if (isKeyPressed(Keyboard::Scan::Right)) {
            cout << "hello" << endl;
        }

        // Handle key presses only when in focus
        if (window.hasFocus()) {
            // if the left button is down, give bg a lil push
            if (isButtonPressed(Mouse::Button::Left)) {
                game.players[0]->pos.x += 1;
                // Character moves left, screen moves right
                if (Mouse::getPosition(window).x < window.getSize().x / 2) {
                    bg_sprite.move({5, 0});
                }
                // character moves right, screen moves left
                else
                    bg_sprite.move({-5, 0});
            }
        }


        // clear the window with black color
        window.clear(Color::Black);

        // DRAW EVERYTHING HERE
        window.draw(bg_sprite);
        for (Player *i : game.players) {
            i->self.setPosition(i->pos);
            window.draw(i->self);
        }
        // window.draw(...);


        // end the current frame
        window.display();
    }
    return 0;
}
