#include <SFML/Graphics.hpp>
using namespace std;
#include <iostream>
#include <string>
#include <sys/time.h>

const int ballRadius = 10;

const int playerYLength = 150;
const int playerXLength = 10;

int ballSpeed = 6;

int playerScore = 0;
int botScore = 0; 

bool detect_collision(float ballX, float ballY, float playerX, float playerY);
bool players_turn();

int main()
{
    // Create the window
    const int screenX = 800;
    const int screenY = 600;
    sf::RenderWindow window(sf::VideoMode(screenX, screenY), "Pong");

    // Player variables
    int playerSpeed = 20;
    float playerY = 200.0f;
    float playerX = 700.f;

    // Bot variables
    float botY = 200.f;
    float botX = 100.f;

    // Ball variables
    float ballX = 400.0f;

    float secondsSinceLastCollision = 0.3f;
    unsigned long int lastCollisionTime = 0;

    // Run the program as long as the window is open
    while (window.isOpen()) {
        // Check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            // Key pressed
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

        // Player score
        sf::Font font;
        font.loadFromFile("calibri.ttf");
        sf::Text playerScoreText;
        playerScoreText.setFont(font); // font is a sf::Font
        playerScoreText.setString(to_string(playerScore));
        playerScoreText.setCharacterSize(60); // in pixels, not points!
        playerScoreText.move(420.f, 70.0f);
        window.draw(playerScoreText);

        // Bot score 
        sf::Text botScoreText;
        botScoreText.setFont(font); // font is a sf::Font
        botScoreText.setString(to_string(botScore));
        botScoreText.setCharacterSize(60); // in pixels, not points!
        botScoreText.move(350.f, 70.0f);
        window.draw(botScoreText);

        // Draws the middle line (where the ball spawns)
        sf::RectangleShape middleLine(sf::Vector2f(800.f, 1.f));
        middleLine.rotate(90.f);
        middleLine.move(400.f, 0.0f);
        window.draw(middleLine);

        // Ball
        sf::CircleShape ball(ballRadius);
        ball.move(ballX, 300.f);
        ballX = ballX + ballSpeed;
        sf::Vector2f ballPosition = ball.getPosition();
        float ballX = ballPosition.x;
        float ballY = ballPosition.y;

        // Draws the player line
        sf::RectangleShape playerLine(sf::Vector2f(playerYLength, playerXLength));
        playerLine.rotate(90.f);
        playerLine.move(playerX, playerY);
        window.draw(playerLine);

        // Draws the bot line
        sf::RectangleShape botLine(sf::Vector2f(playerYLength, playerXLength));
        botLine.rotate(90.f);
        botLine.move(botX, botY);
        window.draw(botLine);

        unsigned long int currentTime = time(NULL);

        // Bot collision
        bool botCollision = detect_collision(ballX, ballY, botX, botY);
        if (botCollision && currentTime - lastCollisionTime > secondsSinceLastCollision) {
            ballSpeed = ballSpeed * -1;
            lastCollisionTime = time(NULL);
        }

        // Player collision
        bool playerCollision = detect_collision(ballX, ballY, playerX, playerY);
        if (playerCollision && currentTime - lastCollisionTime > secondsSinceLastCollision) {
            ballSpeed = ballSpeed * -1;
            lastCollisionTime = time(NULL);
        }

        // If ball out of bounds (lost point)
        if (ballX > screenX) { 
            ;
        }
        
        window.draw(ball);
        window.display(); // Frame
    }

    return 0;
}

bool detect_collision(float ballX, float ballY, float playerX, float playerY) {

    // https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection
    bool collisionX = playerX + playerXLength >= ballX && ballX + ballRadius >= playerX; // x-axis collision
    bool collisionY = playerY + playerYLength >= ballY && ballY + ballRadius >= playerY; // y-axis collision

    return collisionX && collisionY; // If collision on both axis
}

// Returns true if it's the player's turn
bool players_turn() {
    if (ballSpeed > 0) return true;
    else return false;
}