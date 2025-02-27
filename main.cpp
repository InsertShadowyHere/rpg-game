#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <unistd.h>
#include <limits.h>
#include <iostream>

using namespace std;

/* This is the Camera class
 * I'm not sure we need it yet but I made it anyways just in case
 * Our goal for the camera is just to be able to change the position of all images in the world
 * relative to our character
 * We might end up using a player method for this though, im not sure
 *
 * Movement is handled like this:
 * When the player moves left, the scene moves right by the same amount
 *
 */

/// Handles player data, BLOBbing, and such
///
/// @param id int
/// @param uname string
class Player {
public:
    int id = 1;
    string uname;
    long data = 1;

    long blob() {
        data += 1;
        cout << "Player " << id << " data: " << data << endl;
        return 0;
    }

    Player(string uname) {
        id = next_id();
        Player::uname = uname;

    }
private:
    int next_id() {
        return 0;
    }
};


int main() {
    const size_t size = 1024;
    // Allocate a character array to store the directory path
    char buffer[size];

    // Call _getcwd to get the current working directory and store it in buffer
    if (getcwd(buffer, size) != NULL) {
        // print the current working directory
        cout << "Current working directory: " << buffer << endl;
    }
    else {
        // If _getcwd returns NULL, print an error message
        cerr << "Error getting current working directory" << endl;
    }
    const string x = "I'm a string!";
    cout << x << std::endl;
    Player player("Davis");
    player.blob();
    player.blob();
    sf::Texture bg_texture("assets/bg.png");
    sf::Sprite bg_sprite(bg_texture);

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
    while (window.isOpen())
    {
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
                sprite.move({5,-5});
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