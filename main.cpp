#include <SFML/Graphics.hpp>
using namespace std;
#include <iostream>
#include <string>
#include <sys/time.h>

const int ballRadius = 10;

const int playerYLength = 150;
const int playerXLength = 10;

// Player variables
int playerSpeed = 40;
float playerY = 200.0f;
float playerX = 700.f;

// Ball variables
float ballX = 400.0f;
float ballY = 300.f;

float ballSpeed = 9;
float ballYSpeed = 0;

int playerScore = 0;
int botScore = 0; 

bool detect_collision(float ballX, float ballY, float objX, float objY);
bool players_turn();

int main() {
    // Create the window
    const int screenX = 800;
    const int screenY = 600;

    sf::RenderWindow window(sf::VideoMode(screenX, screenY), "Pong");
    window.setFramerateLimit(60); // call it once, after creating the window

    sf::Font font;
    font.loadFromFile("calibri.ttf");

    // Bot variables
    float botY = 200.f;
    float botX = 100.f;

    float secondsSinceLastCollision = 0.3f;
    unsigned long int lastCollisionTime = 0;

    // Run the program as long as the window is open
    while (window.isOpen()) {
        // Check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            // Key pressed
            else if (event.type == sf::Event::KeyPressed) {

                char up = 'I';
                char down = 'J';

                char tpd = event.key.code;
                int newPosY;
                bool outOfBounds = false;
                if (tpd == up) {
                    newPosY = playerY - playerSpeed;
                    if (newPosY <= 0 + playerSpeed * -1) {
                        outOfBounds = true;
                    }
                } else if (tpd == down) {
                    newPosY = playerY + playerSpeed;
                    if (newPosY + playerYLength >= screenY + playerSpeed) {
                        outOfBounds = true;
                    }
                }
                if (!outOfBounds) {
                    playerY = newPosY;
                }
            }
        }

        // Clear the window with black color
        window.clear(sf::Color::Black);

        // Player score
        sf::Text playerScoreText;
        playerScoreText.setFont(font); // font is a sf::Font
        playerScoreText.setString(to_string(playerScore));
        playerScoreText.setCharacterSize(60); // in pixels, not points!
        playerScoreText.move(420.f, 70.0f);

        // Bot score 
        sf::Text botScoreText;
        botScoreText.setFont(font); // font is a sf::Font
        botScoreText.setString(to_string(botScore));
        botScoreText.setCharacterSize(60); // in pixels, not points!
        botScoreText.move(350.f, 70.0f);

        // Draws the middle line (where the ball spawns)
        sf::RectangleShape middleLine(sf::Vector2f(800.f, 1.f));
        middleLine.rotate(90.f);
        middleLine.move(400.f, 0.0f);

        // Ball
        sf::CircleShape ball(ballRadius);
        ball.move(ballX, ballY);
        ballX = ballX + ballSpeed;
        ballY = ballY + ballYSpeed;
        sf::Vector2f ballPosition = ball.getPosition();

        // Draws the player line
        sf::RectangleShape playerLine(sf::Vector2f(playerYLength, playerXLength));
        playerLine.rotate(90.f);
        playerLine.move(playerX, playerY);

        // Draws the bot line
        sf::RectangleShape botLine(sf::Vector2f(playerYLength, playerXLength));
        botLine.rotate(90.f);
        botLine.move(botX, botY);

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



        // If ball out of bounds (vertical) (lost point)
        if (ballX > screenX || ballX < 0) { 

            bool playersTurn = players_turn();
            if (playersTurn) botScore++;
            else playerScore++;
            int totalScore = playerScore + botScore;

            ballYSpeed = 0;

            if (totalScore % 2 == 0) ballSpeed = ballSpeed * -1; // Flips direction

            ballX = 400.0f; // Returns to default
            ballY = 300.f; // Returns to default
        } 
        if (ballY > screenY || ballY < 0) {
            ballYSpeed *= -1;
        }
        
        window.draw(botScoreText);
        window.draw(middleLine);
        window.draw(playerLine);
        window.draw(botLine);
        window.draw(playerScoreText);
        window.draw(ball);

        window.display(); // Frame
    }

    return 0;
}

bool detect_collision(float lBallX, float lBallY, float objX, float objY) {

    // https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection
    bool collisionX = objX + playerXLength >= lBallX && lBallX + ballRadius >= objX; // x-axis collision
    bool collisionY = objY + playerYLength >= lBallY && lBallY + ballRadius >= objY; // y-axis collision
    bool collision = collisionX && collisionY;

    if (collision) {
        float hit = (((objY + playerYLength) - lBallY) - 75) * -1; // From -75 to 75
        float speedYPlus = (hit * 0.04);
        ballYSpeed += speedYPlus;
    }

    return collision; // If collision on both axis
}

// Returns true if it's the player's turn
bool players_turn() {
    if (ballSpeed > 0) return true;
    else return false;
}
