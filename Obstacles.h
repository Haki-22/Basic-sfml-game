#ifndef OBSTACLES_H
#define OBSTACLES_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <random>

class Obstacle {
private:

    sf::Sprite obstacleSprite;
    sf::Texture obstacleTexture;
    sf::Vector2f scaleSize;
    sf::RenderWindow window;
    float speed = 1;
    float floor;
    float difficulty = 1;
    float lower_bound = 0.1;
    float upper_bound = 0.30;
    float random_float = lower_bound + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (upper_bound - lower_bound)));
public:
    Obstacle(float speed, float difficulty, float floor, sf::RenderWindow& window);
    void setPosition(float x, float y);
    sf::Vector2f getPosition();
    sf::FloatRect getGlobalBounds();
    void update(float speed, float difficulty, float floor, sf::RenderWindow& window, bool fullscreen);
    void respawn(sf::RenderWindow& window, float floor, bool& collisionOnce, int& score);
    void draw(sf::RenderWindow& window);
    void move(bool& fullscreen);
    void scale(float scale);
    void updateSpeed(float speed);
    bool pixelPerfectColission(sf::Sprite& playerSprite);

    ~Obstacle();
};

#endif	/* OBSTACLES_H */