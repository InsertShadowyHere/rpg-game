#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

using namespace std;

/* This is the Camera class
 * I'm not sure we need it yet but I made it anyways just in case
 * Our goal for the camera is just to be able to change the position of all images in the world
 * relative to our character
 * We might end up using a player method for this though, im not sure
 */
class Camera {
public:
    int x;
    int y;
    Camera(int xc, int yc) {
        x = xc;
        y = yc;
    }
};

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


int main()
{
    const string x = "I'm a string!";
    cout << x << std::endl;
    Player player("Davis");
    player.blob();
    player.blob();

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
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();

            // Handle key presses only when in focus
            if (window.hasFocus()) {
                // If mouse button is pressed:
                // move Jared
                // Set window position
                if (event->is<sf::Event::MouseButtonPressed>()) {

                    sprite.move({10, -10});
                    window.setPosition({10, 10});
                }

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)) {
                cout << "hello" << endl;
            }
        }

        // if the left button is down, give jared a lil push
        if (isButtonPressed(sf::Mouse::Button::Left)) {
            sprite.move({1, -1});
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
        window.draw(sprite);

        // end the current frame
        window.display();
    }
    return 0;
}