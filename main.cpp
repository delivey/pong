#include <SFML/Graphics.hpp>
using namespace std;
#include <iostream>
#include <string>

bool detect_collision(float ballX, float ballY, float playerX, float playerY) {
    return true;
}

int main()
{
    // Create the window
    const int screenX = 800;
    const int screenY = 600;
    sf::RenderWindow window(sf::VideoMode(screenX, screenY), "Pong");

    // Player variables
    const int playerSpeed = 20;
    float playerY = 200.0f;
    float playerX = 700.f;

    // Ball variables
    float ballX = 400.0f;
    const int ballSpeed = 4;

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
                    playerY = playerY - playerSpeed;
                } else if (tpd == down) {
                    playerY = playerY + playerSpeed;
                }
            }
        }

        // Clear the window with black color
        window.clear(sf::Color::Black);

        // Draws the player line
        sf::RectangleShape playerLine(sf::Vector2f(150.f, 10.f));
        playerLine.rotate(90.f);
        playerLine.move(playerX, playerY);
        window.draw(playerLine);

        // Draws the middle line (where the ball spawns)
        sf::RectangleShape middleLine(sf::Vector2f(800.f, 1.f));
        middleLine.rotate(90.f);
        middleLine.move(400.f, 0.0f);
        window.draw(middleLine);

        // Ball
        sf::CircleShape ball(10.f);
        ball.move(ballX, 300.f);
        ballX = ballX + ballSpeed;

        // Ball position
        sf::Vector2f ballPosition = ball.getPosition();

        float ballX = ballPosition.x;
        float ballY = ballPosition.y;
        cout << "Ball " << ballX << " " << ballY << endl;
        cout << "Player " << playerX << " " << playerY << endl;

        bool collision = detect_collision(ballX, ballY, playerX, playerY);

        if (ballX > screenX) {
            ;
        } else if (collision) {
            cout << "Collision" << endl;
        } else {
            window.draw(ball);
        }

        window.display(); // Frame

    }

    return 0;
}