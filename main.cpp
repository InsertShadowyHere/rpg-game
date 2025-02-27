#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <cstdlib>

using namespace std;

int main()
{
    const string x = "I'm a string!";
    cout << x << std::endl;

    // create the window and initialize some things
    sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");
    window.setFramerateLimit(60); // call it once after creating the window

    // test image

    sf::Texture texture("assets/mean-girls-guy-rmbg.png", false, sf::IntRect({0,0}, {320, 320}));
    sf::Sprite sprite(texture);


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

                    sprite.setPosition({sprite.getPosition().x, sprite.getPosition().y+10});
                    window.setPosition({10, 10});
                }




            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)) {
                cout << "hello" << endl;
            }
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