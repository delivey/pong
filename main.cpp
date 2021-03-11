#include <SFML/Graphics.hpp>
using namespace std;
#include <iostream>
#include <string>

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");

    const int speed = 20;
    float yAxis = 200.0f;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            // key pressed
            else if (event.type == sf::Event::KeyPressed) {

                char up = 'I';
                char down = 'J';
                char left = 'G';
                char right = 'H';

                char tpd = event.key.code;
                if (tpd == up) {
                    yAxis = yAxis - speed;
                } else if (tpd == down) {
                    yAxis = yAxis + speed;
                }
            }
        }

        // Clear the window with black color
        window.clear(sf::Color::Black);

        // Draws the player line
        sf::RectangleShape playerLine(sf::Vector2f(150.f, 10.f));
        playerLine.rotate(90.f);
        playerLine.move(700.f, yAxis);
        window.draw(playerLine);

        // Draws the player line
        sf::RectangleShape middleLine(sf::Vector2f(1600.f, 1.f));
        middleLine.rotate(90.f);
        middleLine.move(400.f, 0.0f);
        window.draw(middleLine);

        window.display(); // Frame

    }

    return 0;
}