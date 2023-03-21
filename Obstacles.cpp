#include "Obstacles.h"
#include <iostream>
#include "Collision.hpp"

/**
* Creates obstacle object
*
* @param float speed (For same behaviour in different FPS)
* @param float difficulty (To scale game difficulty)
* @param float floor (where should the obstacle be created)
* @param Reference window
* 
*/
Obstacle::Obstacle(float speed, float difficulty, float floor, sf::RenderWindow& window)
{
    this->speed = speed;
    this->scaleSize.x = this->random_float;
    this->scaleSize.y = this->random_float;
    this->difficulty = difficulty;

    if (!this->obstacleTexture.loadFromFile("pictures/cucumber2-copy.png"))
    {
        printf("Texture was not loaded");
    }
   /* else {
        std::cout << "Obstacle texture loaded";
    }*/
    obstacleSprite = sf::Sprite(obstacleTexture);
    obstacleSprite.setScale(scaleSize.x, scaleSize.y);
    obstacleSprite.setPosition(window.getSize().x - obstacleSprite.getGlobalBounds().width, floor - obstacleSprite.getGlobalBounds().height);
    //obstacleSprite.setFillColor(sf::Color::Red);
}

/**
* Sets new obstacle position
*
* @param float x,y (new position)
*
*/
void Obstacle::setPosition(float x, float y)
{
    obstacleSprite.setPosition(x, y);
}

/**
* Get obstacle position
*
* @return sf::Vector2F
*
*/
sf::Vector2f Obstacle::getPosition()
{
    return obstacleSprite.getPosition();
}

/**
* Get obstacle global bounds
*
* @return sf::FloatRect
*
*/
sf::FloatRect Obstacle::getGlobalBounds()
{
    return obstacleSprite.getGlobalBounds();
}


/**
* Updates obstacle (in new window size)
*
* @param float speed (For same behaviour in different FPS)
* @param float difficulty (To scale game difficulty)
* @param float floor (where should the obstacle be moved)
* @param Reference window
* @param bool fullscreen (wether the new window size is fullscreen mode)
*
*/
void Obstacle::update(float speed, float difficulty, float floor, sf::RenderWindow& window, bool fullscreen)
{   

    if (fullscreen) {
        this->lower_bound = 0.4;
        this->upper_bound = 0.9;
        //obstacleSprite.scale(2, 2);
        //obstacleSprite.setPosition()
    }
    else {
        //obstacleSprite.setScale(obstacleSprite.getScale().x / 2, obstacleSprite.getScale().y / 2);
        this->lower_bound = 0.1;
        this->upper_bound = 0.3;
        if (window.getSize().y > 750) {
            this->lower_bound = 0.2;
            this->upper_bound = 0.55;
        }
        if (window.getSize().y > 900) {
            this->lower_bound = 0.4;
            this->upper_bound = 0.7;
        }

    }

    this->floor = floor;
    //this->speed = speed;
    this->difficulty = difficulty;
    if (obstacleSprite.getPosition().x > 400) { //Otherwise you coulf use it for despawning obstacles
        obstacleSprite.setPosition(window.getSize().x, floor - obstacleSprite.getGlobalBounds().height);
    }
    else {
        obstacleSprite.setPosition(obstacleSprite.getPosition().x, floor - obstacleSprite.getGlobalBounds().height);
    }
}

/**
* Gives 100 score if player didnt collide with the obstacle and
* Spawns new obstacle if it goes out of window size x
* 
* 
* @param Reference window
* @param float floor (where should the obstacle be moved)
* @param bool& collisionOnce (for updating score only once)
* @param int& score (for updating score)
*/
void Obstacle::respawn(sf::RenderWindow& window, float floor, bool& collisionOnce, int& score)
{
    if (obstacleSprite.getPosition().x + obstacleSprite.getGlobalBounds().width < 0) {
        random_float = lower_bound + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (upper_bound - lower_bound)));
        obstacleSprite.setScale(random_float, random_float);
        if (score > 100) {
            obstacleSprite.scale(1.4,1.4);
        }
        obstacleSprite.setPosition(window.getSize().x, floor - obstacleSprite.getGlobalBounds().height);
        if (!collisionOnce) {
            score += 10;
        }
        collisionOnce = false;
    }
}


/**
* Draws the obstacle to reference window
* @param Reference window&
*/
void Obstacle::draw(sf::RenderWindow& window)
{
    window.draw(obstacleSprite);   
}

/**
* Moves the obstacle
* @param bool fullscreen (obstacle moves faster when in fullscreen)
*/
void Obstacle::move(bool & fullscreen)
{
    
    if (fullscreen) {
        obstacleSprite.move( (- difficulty * 2)* speed * 5, 0);
    }
    else {
        obstacleSprite.move( (- difficulty * 2) * speed * 5, 0);
    }
}

/**
* Scales the obstacle
* @param float scale (scales obstacle x and y)
*/
void Obstacle::scale(float scale)
{
    obstacleSprite.scale(scale, scale);
}

/**
* Updates obstacle speed
* @param float speed (for same behaviour in different fps)
*/
void Obstacle::updateSpeed(float speed)
{
    this->speed = speed;
}

/**
* Cheks wether the player collided with the obstacle (Pixel perfect)
* @param sprite (The player)
* @returns true if player collided
*/
bool Obstacle::pixelPerfectColission(sf::Sprite& playerSprite) {
    if (Collision::PixelPerfectTest(playerSprite, obstacleSprite)) {
        //std::cout << "Collsion";
        return true;
    }
    else {
        return false;
    }
}



Obstacle::~Obstacle(){}