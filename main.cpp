#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <random>
#include <thread>

using namespace sf;

Packet queue_packets() {
    UdpSocket socket;
    if (socket.bind(54000) != sf::Socket::Status::Done)
    {
        std::cout << "Error" << std::endl;
    }
    Packet packet;
    std::optional<IpAddress> sender;
    unsigned short port;
    if (socket.receive(packet, sender, port) != Socket::Status::Done)
    {
        // error...
    }
    std::cout << "Received " << packet.getData() << " from " << sender->toString() << " on port " << port << std::endl;
    return packet;

}

using namespace std;

/// Handles player data, blobbing, and such
///
/// @param id int
/// @param uname string
class Player {
public:
    int id = 1;
    string uname;
    long data = 1;

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
    vector<Player*> players;

    // Fetch player data, add player to vector
    void add_player(const string &uname) {
        auto *player = new Player(uname);
        players.push_back(player);
    }

    void test_player_vector() {
        for (const Player *i : players) {
            std::cout << "Player " << i->id << ": " <<  players[0]->uname << std::endl;
        }

    }


private:

};


int main() {
    const string x = "I'm a string!";
    cout << x << std::endl;
    Player player("Davis");
    sf::Texture bg_texture("assets/bg.png");
    sf::Sprite bg_sprite(bg_texture);
    Game game = Game();
    game.add_player("Davis");
    game.test_player_vector();

    // create the window and initialize some things
    sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");
    window.setFramerateLimit(60); // call it once after creating the window

    // test image
    sf::Texture texture("assets/jared.png");
    sf::Sprite sprite(texture);
    sprite.setPosition({0, 380});


    /* Main loop; run program while window is open.
     * Check for events
     * Take server data
     * Send client data
     * update things that need to be updated
     * Render background
     * Render entities */
    //std::thread receiver(queue_packets());
    while (window.isOpen()) {
        // Check all window events triggered this run-through of the loop
        while (const std::optional event = window.pollEvent()) {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();

            // Handle key presses only when in focus
            if (window.hasFocus()) {
                // If mouse button is pressed:
                // move Jared
                // Set window position
                if (event->is<sf::Event::MouseButtonPressed>()) {
                }
            }
        }
        if (isKeyPressed(sf::Keyboard::Scan::Right)) {
            cout << "hello" << endl;
        }

        // if the left button is down, give bg a lil push
        if (isButtonPressed(sf::Mouse::Button::Left)) {
            // Character moves left, screen moves right
            if (sf::Mouse::getPosition(window).x < window.getSize().x / 2) {
                sprite.move({5, -5});
                bg_sprite.move({-5, 0});
            }
            // character moves right, screen moves left
            else
                bg_sprite.move({5, 0});
        }


        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
        window.draw(sprite);
        window.draw(bg_sprite);

        // end the current frame
        window.display();
    }
    return 0;
}
