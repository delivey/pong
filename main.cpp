#include <SFML/Graphics.hpp>
using namespace std;
#include <iostream>
#include <string>
#include <sys/time.h>

#include "src/bounce.h"
#include "src/bot.h"

// Player
int playerSpeed = 40;
float playerY = 200.0f;
float playerX = 700.f;
const int playerYLength = 150;
const int playerXLength = 10;

// Bot
float botY = 200.f;
float botX = 100.f;

// Ball
const float startingBallX = 400.0f;
const float startingBallY = 300.f;
float ballX = startingBallX;
float ballY = startingBallY;
const float baseBallSpeed = 9;
float ballSpeed = baseBallSpeed;
float ballYSpeed = 0;
const int ballRadius = 10;

// Score
int playerScore = 0;
int botScore = 0;

// Window
const int screenX = 800;
const int screenY = 600;

// Collision
float secondsSinceLastCollision = 0.3f;
unsigned long int lastCollisionTime = 0;

// Game
int winningScore = 11;
bool gameEnded = false;
bool isPlayersBall = true;

bool detect_collision(float ballX, float ballY, float objX, float objY);
bool players_turn();
void bounce(float &ballY, int screenY, int ballRadius, float &ballYSpeed);
void out_of_bounds();
void handle_movement(char tpd);
void draw(sf::RenderWindow &window, sf::Font &font);
void handle_collisions();
void handle_events(sf::RenderWindow &window);
void move_bot(float &ballY, float &botY, int screenY, int playerYLength);

int main()
{
    sf::RenderWindow window(sf::VideoMode(screenX, screenY), "Pong"); // Create window
    window.setVerticalSyncEnabled(true);                              // Enable V-Sync

    sf::Font font;
    font.loadFromFile("calibri.ttf");

    // Run the program as long as the window is open
    while (window.isOpen())
    {
        if (!gameEnded)
        {
            handle_events(window);
            handle_collisions();
            out_of_bounds();
            bounce(ballY, screenY, ballRadius, ballYSpeed); // no global
            move_bot(ballY, botY, screenY, playerYLength);  // no global
        }
        draw(window, font);
    }

    return 0;
}

bool detect_collision(float lBallX, float lBallY, float objX, float objY)
{

    // https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection
    bool collisionX = objX + playerXLength >= lBallX && lBallX + ballRadius >= objX; // x-axis collision
    bool collisionY = objY + playerYLength >= lBallY && lBallY + ballRadius >= objY; // y-axis collision
    bool collision = collisionX && collisionY;

    if (collision)
    {
        float hit = (((objY + playerYLength) - lBallY) - playerYLength / 2) * -1; // From -75 to 75
        float speedYPlus = hit * 0.04;
        ballYSpeed += speedYPlus;
    }

    return collision; // If collision on both axis
}

// Returns true if it's the player's turn
bool players_turn()
{
    if (ballSpeed > 0)
        return true;
    else
        return false;
}

// Checks if ball is out of bounds
void out_of_bounds()
{
    if (ballX > screenX || ballX < 0)
    {

        bool playersTurn = players_turn();
        if (playersTurn)
            botScore++;
        else
            playerScore++;

        if (playerScore == winningScore)
        {
            gameEnded = true;
        }
        else if (botScore == winningScore)
        {
            gameEnded = true;
        }

        ballYSpeed = 0;

        if (isPlayersBall)
        {
            ballSpeed = baseBallSpeed * -1; // Flips starting ball direction
            isPlayersBall = false;
        }
        else
        {
            ballSpeed = baseBallSpeed;
            isPlayersBall = true;
        }

        ballX = startingBallX; // Returns to default
        ballY = startingBallY; // Returns to default
    }
}

void handle_movement(char tpd)
{
    char up = 'I';
    char down = 'J';

    int newPosY;
    bool outOfBounds = false;
    if (tpd == up)
    {
        newPosY = playerY - playerSpeed;
        if (newPosY <= 0 + playerSpeed * -1)
        {
            outOfBounds = true;
        }
    }
    else if (tpd == down)
    {
        newPosY = playerY + playerSpeed;
        if (newPosY + playerYLength >= screenY + playerSpeed)
        {
            outOfBounds = true;
        }
    }
    if (tpd == up || tpd == down)
    {
        if (!outOfBounds)
        {
            playerY = newPosY;
        }
    }
}

void draw(sf::RenderWindow &window, sf::Font &font)
{
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

    // Draws the player line
    sf::RectangleShape playerLine(sf::Vector2f(playerYLength, playerXLength));
    playerLine.rotate(90.f);
    playerLine.move(playerX, playerY);

    // Draws the bot line
    sf::RectangleShape botLine(sf::Vector2f(playerYLength, playerXLength));
    botLine.rotate(90.f);
    botLine.move(botX, botY);

    // Clear the window with black color
    window.clear(sf::Color::Black);

    if (!gameEnded)
    {
        window.draw(botScoreText);
        window.draw(playerScoreText);

        window.draw(middleLine);
        window.draw(playerLine);
        window.draw(botLine);
        window.draw(ball);
    }
    else
    {
        string result;
        if (botScore > playerScore)
            result = "You lost!";
        else
            result = "You won!";

        sf::Text resultText;
        resultText.setFont(font);
        resultText.setString(result);
        resultText.setCharacterSize(60);
        resultText.move(300.f, 70.0f);
        window.draw(resultText);

        string fullScore;
        fullScore = to_string(botScore) + " - " + to_string(playerScore);

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString(fullScore);
        scoreText.setCharacterSize(40);
        scoreText.move(360.f, 150.0f);
        window.draw(scoreText);
    }
    window.display();
}

void handle_collisions()
{
    unsigned long int currentTime = time(NULL);

    bool botCollision = detect_collision(ballX, ballY, botX, botY);
    bool playerCollision = detect_collision(ballX, ballY, playerX, playerY);

    if (botCollision || playerCollision)
    {
        if (currentTime - lastCollisionTime > secondsSinceLastCollision)
        {
            ballSpeed = ballSpeed * -1;
            lastCollisionTime = currentTime;
        }
    }
}

void handle_events(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        // Close window
        if (event.type == sf::Event::Closed)
            window.close();

        // Key pressed
        if (event.type == sf::Event::KeyPressed)
        {
            char tpd = event.key.code;
            handle_movement(tpd);
        }
    }
}