#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
//#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Collision.hpp"
#include "Obstacles.h"
#include <string.h>


//void debugCout() {
//    std::cout << "something" << "\n";
//}
//
//void debugFPS(float dt) {
//    //FPS:
//    std::cout << "FPS:" << 1.0f / dt << std::endl;
//}

/**
* Draws boundaries around the sprite, usefull for debugging.
*
* @param Sprite around which the boundaries should be drawn
* @param Reference window
* @returns nothing, draws red boundaries around sprite
*/
void draw_sprite_boundaries(sf::Sprite& sprite, sf::RenderWindow& window) {
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    sf::VertexArray line(sf::LinesStrip, 5);

    line[0].position = sf::Vector2f(spriteBounds.left - 1, spriteBounds.top - 1);
    line[1].position = sf::Vector2f(spriteBounds.left - 1, spriteBounds.top + spriteBounds.height + 1);
    line[2].position = sf::Vector2f(spriteBounds.left + spriteBounds.width + 1, spriteBounds.top + spriteBounds.height + 1);
    line[3].position = sf::Vector2f(spriteBounds.left + spriteBounds.width + 1, spriteBounds.top - 1);
    line[4].position = sf::Vector2f(spriteBounds.left - 1, spriteBounds.top - 1);
    line[0].color = sf::Color::Red;
    line[1].color = sf::Color::Red;
    line[2].color = sf::Color::Red;
    line[3].color = sf::Color::Red;
    line[4].color = sf::Color::Red;

    window.draw(line);
}

/**
* Calculates texture scale for new window.
*
* @param Reference window
* @param Texture
* @returns Vector2f by which the texture should be scaled
*/
sf::Vector2f calculateScaleFactor(sf::RenderWindow& window, sf::Texture& texture) {
    sf::Vector2f ScaleFactor;
    ScaleFactor.x = static_cast<float>(window.getSize().x) / static_cast<float>(texture.getSize().x);
    ScaleFactor.y = static_cast<float>(window.getSize().y) / static_cast<float>(texture.getSize().y);
    return ScaleFactor;
}

/**
* Checks whether user clicked.
*
* @param Reference window
* @returns nothing, prints on console where user clicked
*/
void is_clicked(sf::RenderWindow& window) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        std::cout << "Click at (" << mousePos.x << ", " << mousePos.y << ")\n";
    }
}

/**
* Checks whether text was clicked.
*
* @param Text which to check 
* @param Reference window

* @return false if text was clicked
*/
bool buttonClickDetect(sf::Text& text, sf::RenderWindow& window) {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::FloatRect textBounds = text.getGlobalBounds();

    sf::Vector2f world_mouse_pos = window.mapPixelToCoords(mousePosition);

    if (textBounds.contains(world_mouse_pos.x, world_mouse_pos.y))
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            return false;
        }
        else {
            return true;
        }
    }

    /**
 * Checks whether sprite was clicked.
 *
 * @param Sprite which to check 
 * @param Reference window 
 * @param bool to check only one click
 * @return true if sprite was clicked
 */
}bool colision_detect_bool(sf::Sprite& sprite, sf::RenderWindow& window, bool& clickedOnce) {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();

    sf::Vector2f world_mouse_pos = window.mapPixelToCoords(mousePosition);

    if (spriteBounds.contains(world_mouse_pos.x, world_mouse_pos.y))
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !clickedOnce)
        {
            //printf("Colision Click! \n");
            clickedOnce = true;
            return true;
            //std::cout << sprite.getGlobalBounds().left << sprite.getGlobalBounds().top;
        }
        else {
            //printf("Colision\n");
            //std::cout << "X: (mouse_pos, bounds_pos)" << mousePosition.x << ", " << spriteBounds.width << "y: (mouse_pos, bounds_pos)" << mousePosition.y << ", " << spriteBounds.height;
            return false;
        }
    }
    else {
        return false;
    }
}

/*
Move player up or down
 */
void move_sprite(sf::Sprite& sprite, float speed, sf::Vector2f& basePlayerPos, sf::Vector2f& playerVelocity, bool& isJumping, sf::RenderWindow& window, bool& doubleJump, bool& jumpWasPressed) {


    sf::Vector2f position = sprite.getPosition();

    // Initialize player velocity to zero
    playerVelocity.x = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if (!isJumping) {
                playerVelocity.y = -speed;
                isJumping = true;
                doubleJump = false;
            }

        else {
            if (!doubleJump && playerVelocity.y >0 ) {
                playerVelocity.y = -1;
                doubleJump = true;
                //std::cout << "\nSkok 2\n";
            }
            jumpWasPressed = false;
            //std::cout << "doublejump = " << std::boolalpha << doubleJump << "\n";
        }   
    }

    // Handle jumping physics
    if (isJumping) {
        //If jump speed is up (-value)
        if (playerVelocity.y < 0) {
            // Moving up
            if (sprite.getPosition().y > 10) {
                playerVelocity.y = -speed * 15;
            }
            else {
                playerVelocity.y =0;
            }

            if (sprite.getPosition().y <= 0) {
                playerVelocity.y = 0;
                sprite.setPosition(sprite.getPosition().x, 10);
            }
        }

        else {
            // Moving down
            jumpWasPressed = false;
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || !sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                if (sprite.getPosition().y <= basePlayerPos.y) {
                    playerVelocity.y = speed * 7;
                    if ((sprite.getPosition().y) > (window.getSize().y / 5)) {
                        playerVelocity.y += playerVelocity.y / 2;
                        if ((sprite.getPosition().y) > (window.getSize().y / 5) * 2) {
                            playerVelocity.y += playerVelocity.y / 2;
                            if ((sprite.getPosition().y) > (window.getSize().y / 5) * 3) {
                                playerVelocity.y += playerVelocity.y / 2;
                            }
                        }
                    }
                }
                else {
                    playerVelocity.y = 0;
                    sprite.setPosition(basePlayerPos);
                    isJumping = false;
                    doubleJump = false;
                }
            }

            else {
                if (sprite.getPosition().y <= basePlayerPos.y) {
                    playerVelocity.y = 0;
                    sprite.setPosition(basePlayerPos);
                    isJumping = false;
                    doubleJump = false;

                }
            }
        }

        position += playerVelocity;
        sprite.setPosition(position);
    }
    else {
        sprite.setPosition(basePlayerPos);
    }


    //Down
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        if (isJumping) {
            if (sprite.getPosition().y <= basePlayerPos.y) {
                playerVelocity.y = speed * 10;
                position += playerVelocity;
                sprite.setPosition(position);
            }
        }
    }
}


int main()
{
    //Load text font
    sf::Font font;
    if (!font.loadFromFile("ARCADECLASSIC.TTF")) {
        std::cerr << "Failed to load font" << std::endl;
    }

    //Create sound buffer
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("mixkit-angry-cartoon-kitty-meow-94.wav")) {
        std::cerr << "Failed to load sound file" << std::endl;
    }

    //Creat meow sound
    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.setVolume(30);  // 30% volume

    srand(time(NULL));
    sf::Vector2f playerVelocity(0.0f, 0.0f);

    sf::RenderWindow window(sf::VideoMode(1024, 640), "Cat game", sf::Style::Default);            //1024x640, 1366x768, 1920x1080

    sf::Vector2u windowSize = window.getSize();
    sf::Vector2i windowPos = window.getPosition();
    sf::Vector2f backgroundScaleFactor(windowSize.x, windowSize.y);
    sf::Vector2f world_mouse_pos;
    sf::Vector2i mousePosition;

    //New bottom (where the cat is moving)
    float floor = windowSize.y / 4 * 3;

    //FPS:
    window.setFramerateLimit(144);

    //Vsync:
    //window.setVerticalSyncEnabled(true);


    //
    //
    // 
    //Background
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("pictures/background.jpg")){printf("playerTexture was not loaded");}

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(calculateScaleFactor(window, backgroundTexture));


    //
    // 
    // 
    //Moving bottom:
    sf::Vector2u backgroundTextureSize = backgroundTexture.getSize();
    //load only moving part:
    //sf::IntRect bottomRect(0, (backgroundTextureSize.y / 4 * 3) + 10, backgroundTextureSize.x / 4, (backgroundTextureSize.y / 5));
    sf::IntRect bottomRect(0, 1490, windowSize.x, 430); 

    // Create a sprite for the bottom part of the playerTexture
    sf::Sprite movingPathSprite;
    movingPathSprite.setTexture(backgroundTexture);
    movingPathSprite.setTextureRect(bottomRect);

    movingPathSprite.setPosition(0, floor);
    //movingPathSprite.setScale(((windowSize.x) / (bottomRect.width) + 0.1), 1);


    //
    //
    //
    //Player 
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("pictures/cat.png"))   
    {
        printf("playerTexture was not loaded");
    }
    playerTexture.setSmooth(true);

    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);

    //Load first cat from cat.png
    sf::IntRect catRectangle(0, 0, 400, 180);
    playerSprite.setTextureRect(catRectangle);

    //Rotate cat, set it smaller, reset origin
    playerSprite.setScale(-0.5, 0.5);
    playerSprite.setOrigin(playerSprite.getGlobalBounds().width / 0.5, 0);

    sf::Vector2f basePlayerPos(50, floor - playerSprite.getGlobalBounds().height);
    playerSprite.setPosition(basePlayerPos);

    //
    // 
    // 
    //Lives and score
    sf::Texture livesTexture;
    if (!livesTexture.loadFromFile("pictures/box.png")) { printf("Box not loaded"); }
    livesTexture.setSmooth(true);

    sf::Text livesText("Lives", font, 40);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(20, 0);

    sf::Text scoreText("Score ", font, 40);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(windowSize.x /2 + scoreText.getGlobalBounds().width - 100, 0);
    
    sf::Text scoreNum("000", font, 40);
    scoreNum.setFillColor(sf::Color::White);
    scoreNum.setPosition(windowSize.x / 2 + scoreNum.getGlobalBounds().width + scoreText.getGlobalBounds().width, 0); 
    
    sf::Text fpsText("0", font, 40);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition(window.getSize().x - 150, 0);


    sf::Sprite playerlife1;
    playerlife1.setTexture(livesTexture);
    playerlife1.setScale(0.1,0.1);
    playerlife1.setPosition(150, 0);



    sf::Sprite playerlife2;
    playerlife2.setTexture(livesTexture);
    playerlife2.setScale(0.1, 0.1);
    playerlife2.setPosition(250, 0);

    //std::cout <<  playerlife1.getGlobalBounds().width;


    sf::Sprite playerlife3;
    playerlife3.setTexture(livesTexture);
    playerlife3.setScale(0.1, 0.1);
    playerlife3.setPosition(350, 0);
    
    sf::Sprite lifePlus;
    lifePlus.setTexture(livesTexture);
    lifePlus.setScale(0.15, 0.15);
    lifePlus.setPosition(600, 200);

    

    //
    // 
    // 
    //
     //obstacle, cucumber
    //sf::Texture cucumberTexture;
    //cucumberTexture.setSmooth(true);
    //if (!cucumberTexture.loadFromFile("pictures/cucumber2-copy.png"))
    //{
    //    printf("Texture was not loaded");
    //}

    //sf::Sprite cucumberSprite;
    //cucumberSprite.setTexture(cucumberTexture);
    //cucumberSprite.setScale(0.2, 0.2);
    ////cucumberSprite.rotate(25);
    //cucumberSprite.setPosition(((windowSize.x) - cucumberSprite.getGlobalBounds().width), floor - cucumberSprite.getGlobalBounds().height);



    sf::Clock playerAnimationClock;
    sf::Clock bottomAnimationClock;
    sf::Clock obstaclesAnimationClock;
    sf::Clock lifePlusClock;
    sf::Clock timeClock;
    sf::Time frameTime = sf::seconds(0.1f);
    sf::Time lifeFrame = sf::seconds(4);

    float dt = 0; //DeltaTime
    float multiplier = 60.f;

    float speed = 1;
    float difficulty = 1;
    float lower_bound = 0.1;
    float upper_bound = 0.30;
    float random_float = lower_bound + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (upper_bound - lower_bound)));


    bool pause = false;
    bool pauseonce = false;
    bool wasEscapePressed = false;
    bool jumpWasPressed = false;
    bool fullscreen = false;
    bool isJumping = false;
    bool doubleJump = false;
    bool collisionOnce = false;
    bool lifePlusOnce = false;
    bool clickedOnce = false;

    Obstacle cucumber(speed, difficulty, floor, window);

    //
    // 
    //pause screen:
    sf::Text pausedText("Game Paused", font, 60);
    pausedText.setFillColor(sf::Color::White);

    sf::Text resumeText("Resume", font, 40);
    resumeText.setFillColor(sf::Color::Green);
    
    sf::Text quitText("Quit", font, 40);
    quitText.setFillColor(sf::Color::Red);

    sf::Text howTo("Jump   W/spacebar/arrow up \nJump again\nDown   S/arrow down", font, 25);
    howTo.setFillColor(sf::Color::White);

    sf::Text gameOverText("GAME OVER", font, 80);
    howTo.setFillColor(sf::Color::White);
    
  
    std::string scoreString= "000";

    sf::Texture screenshot;
    screenshot.create(1920,1080);
    sf::Sprite pauseScreen;
    sf::Color grey(128, 128, 128, 200);

    short int playerLives = 3;
    int score = 0;
    int scorecheck = 100;


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::LostFocus) {
                pause = true;
            }
            /*if (event.type == sf::Event::GainedFocus) {
                pause = false, pauseonce = false;
            }*/
            //Pause game by escape
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                if (!wasEscapePressed) {
                    pause = !pause;
                    pauseonce = false;
                    std::cout << "Game_paused = " << std::boolalpha << pause << "\n";
                }
                wasEscapePressed = true;
            }
            else {
                wasEscapePressed = false;
            }
            if (event.type == sf::Event::Resized) {
                windowSize = window.getSize();

                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));

                //Reset default floor and sprite position
                floor = window.getSize().y / 4 * 3;
                basePlayerPos.y = floor - playerSprite.getGlobalBounds().height;
                //speed = dt * multiplier;

                //Set new moving bottom width
                bottomRect.width = windowSize.x;
                if (windowSize.x < 1024) {
                    bottomRect.width = 1024;
                }

                //Reset positions
                fpsText.setPosition(window.getSize().x - fpsText.getGlobalBounds().width, 0);
                playerSprite.setPosition(basePlayerPos);
                movingPathSprite.setPosition(0, floor);
                backgroundSprite.setScale(calculateScaleFactor(window, backgroundTexture));
                cucumber.update(speed, difficulty, floor, window, fullscreen);
                cucumber.updateSpeed(1);

            }

            // Check for the F11 or F key being pressed
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11) || sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                if (!fullscreen) {

                    //Get the position of where window is now
                    windowPos = window.getPosition();
                    windowSize = window.getSize();

                    // Create a new fullscreen window
                    window.create(sf::VideoMode::getFullscreenModes()[0], "Cat Game", sf::Style::Fullscreen);
                    fpsText.setPosition(window.getSize().x - fpsText.getGlobalBounds().width - 50, 0);
                    window.setFramerateLimit(144);

                    //Make cat bigger
                    playerSprite.setScale(-0.6, 0.6);
                    playerSprite.setOrigin(playerSprite.getGlobalBounds().width / .6, .6);

                    //change to fullscreen floor and cat base position
                    floor = 790;
                    basePlayerPos.y = floor - playerSprite.getGlobalBounds().height;

                    //maximize background picture in new window
                    backgroundSprite.setScale(calculateScaleFactor(window, backgroundTexture));

                    //set moving bottom width to new window size
                    bottomRect.width = window.getSize().x;
                    
                    //cucumberSprite.setScale(0.3, 0.3);
                    //cucumberSprite.setPosition(cucumberSprite.getPosition().x, floor - cucumberSprite.getGlobalBounds().height);

                    //set new positions
                    movingPathSprite.setPosition(0, floor);
                    playerSprite.setPosition(basePlayerPos.x, basePlayerPos.y);
                    
                    
                    //speed = dt * multiplier;
                    //Make new pause window
                    /*screenshot.create(window.getSize().x, window.getSize().y);
                    screenshot.update(window);*/

                    fullscreen = true;
                    cucumber.update(speed, difficulty, floor, window, fullscreen);
                    cucumber.updateSpeed(1); 
                }
                else {
                    
                    //Create new screenshot
                    screenshot.create(window.getSize().x, window.getSize().y);
                    screenshot.update(window);

                    //Restore the previous window size and position
                    window.create(sf::VideoMode(windowSize.x, windowSize.y), "Cat Game", sf::Style::Default);
                    window.setPosition(windowPos);
                    window.setFramerateLimit(144);

                    //Reset cat size
                    playerSprite.setScale(-0.5, 0.5);
                    playerSprite.setOrigin(playerSprite.getGlobalBounds().width / 0.5, 0);

                    //Reset default floor and sprite position
                    floor = window.getSize().y / 4 * 3;
                    basePlayerPos.y = floor - playerSprite.getGlobalBounds().height;
                    
                    //Set new moving bottom width
                    bottomRect.width = windowSize.x;
                    if (windowSize.x < 1024) {
                        bottomRect.width = 1024;
                    }

                    //Set new locations
                    fpsText.setPosition(windowSize.x - fpsText.getGlobalBounds().width, 0);
                    playerSprite.setPosition(basePlayerPos);
                    movingPathSprite.setPosition(0, floor);
                    backgroundSprite.setScale(calculateScaleFactor(window, backgroundTexture));
                   
                    //speed = dt * multiplier;
                    
                    fullscreen = false;
                    cucumber.update(speed, difficulty, floor, window, fullscreen);
                    cucumber.updateSpeed(1);
                }
            }
        }
        
        if (!pause && playerLives > 0) {
            //is_clicked(window);

            //create delta time for same behavior in different FPS:
            dt = timeClock.restart().asSeconds();
            speed *= dt * multiplier;
            cucumber.updateSpeed(speed);
            

            //
            // 
            // 
            //Check user input:
            move_sprite(playerSprite, speed, basePlayerPos, playerVelocity, isJumping, window, doubleJump, jumpWasPressed);

            cucumber.respawn(window, floor, collisionOnce, score);
            cucumber.move(fullscreen);
           
            if (cucumber.pixelPerfectColission(playerSprite)) {
                playerSprite.setColor(sf::Color(255,0,0,128));
                if (!collisionOnce) {
                    --playerLives;
                    collisionOnce = true;
                    sound.play();
                }
               
            }
            else {
                playerSprite.setColor(sf::Color(255,255,255,255));
            }
           

            //Set HUD
            scoreText.setPosition(window.getSize().x / 2 - scoreText.getGlobalBounds().width / 2 + 10, 0);
            scoreNum.setPosition(window.getSize().x / 2 - scoreNum.getGlobalBounds().width / 2 + scoreText.getGlobalBounds().width, 0);
            fpsText.setString("Fps "+ std::to_string(static_cast<int>(1.0f / dt)));


            
            // 
            // 
            //Display and animate:
            window.clear();
            window.draw(backgroundSprite);


            //Animate cat:
            if(playerAnimationClock.getElapsedTime() > (frameTime) / (difficulty )) {

                playerAnimationClock.restart();
                
                if (catRectangle.top == 2200) {
                    catRectangle.top = 0;
                    catRectangle.top += 200;
                    playerSprite.setTextureRect(catRectangle);
                }
                else {
                    catRectangle.top += 200;
                    playerSprite.setTextureRect(catRectangle);
                }
                
            }

            //Animate bottom:
            if (bottomAnimationClock.getElapsedTime() > (frameTime / difficulty) / 10.f) {
                bottomAnimationClock.restart();

                if (bottomRect.left >= (backgroundTextureSize.x - bottomRect.width)) {
                    bottomRect.left = 0;  
                    movingPathSprite.setTextureRect(bottomRect);
                }
                else {
                    bottomRect.left +=  1;
                    movingPathSprite.setTextureRect(bottomRect);
                }
                
            } 

            //Display lives:
            window.draw(livesText);
            if (playerLives >= 3) {
                window.draw(playerlife1);
                window.draw(playerlife2);
                window.draw(playerlife3);
                lifePlusOnce = false;
            }
            else if (playerLives == 2) {
                window.draw(playerlife1);
                window.draw(playerlife2);
                if (25 == rand() % 10000) {
                    if (lifePlusClock.getElapsedTime().asSeconds() > 2.f) {
                        lifePlusClock.restart();
                    }
                    lifePlusOnce = true;
                    clickedOnce = false;
                }
            }
            else if (playerLives == 1) {
                window.draw(playerlife1);
                if (25 == rand() % 10000) {
                    if (lifePlusClock.getElapsedTime().asSeconds() > 2.f) {
                        lifePlusClock.restart();
                    }
                    lifePlusOnce = true;
                    clickedOnce = false;
                }
            }

            scoreString = std::to_string(score);
            //draw_sprite_boundaries(playerSprite, window);
            
            //Spawn additional life (1 in 10000 chance in each frame per second (144fps))
            if (lifePlusOnce && lifePlusClock.getElapsedTime().asSeconds() < 2.f) {
                window.draw(lifePlus);
                if (colision_detect_bool(lifePlus, window, clickedOnce)) {
                    ++playerLives;
                    lifePlusOnce = false;
                }
            }

            cucumber.draw(window);
            //window.draw(cucumberSprite);
            window.draw(movingPathSprite);
            window.draw(playerSprite);
            window.draw(scoreText);

            scoreNum.setString(scoreString);
            window.draw(scoreNum);
            window.draw(fpsText);

            window.display();
            
            //Reset DT:
            speed = 1;

            //increase difficulty speed
            if (scorecheck == score) {
                difficulty += 0.1;
                ++score;
                scorecheck += 101;
            }
        }

        if (pause) {
            if (!pauseonce) {
                std::cout << "Game paused (pauseonce)\n";
                //Screenshot game frame
                screenshot.create(window.getSize().x, window.getSize().y);
                //std::cout << window.getSize().x<< " " << window.getSize().y;   //Shows 1920x1080 but if initialized after pause in default window bugs
                screenshot.update(window);

                pauseScreen.setTexture(screenshot);

                //cucumber.setPosition(cucumber.getPosition().x, cucumber.getPosition().y);

                pauseScreen.setColor(grey);

                pausedText.setPosition(window.getSize().x / 2 - pausedText.getGlobalBounds().width / 2, 50);

                resumeText.setPosition(window.getSize().x / 2 - resumeText.getGlobalBounds().width / 2, 200);

                howTo.setPosition(window.getSize().x / 2 - howTo.getGlobalBounds().width / 2, 300);

                quitText.setPosition(window.getSize().x / 2 - quitText.getGlobalBounds().width / 2, window.getSize().y - 100);

                if (cucumber.getPosition().x > window.getSize().x / 2) {
                    cucumber.setPosition(window.getSize().x, cucumber.getPosition().y);
                }


                window.clear();

                window.draw(livesText);
                window.draw(pauseScreen);
                window.draw(pausedText);
                window.draw(resumeText);
                window.draw(howTo);
                window.draw(quitText);

                window.display();

                pauseonce = true;

            }

            //Resume game by clicking
            pause = buttonClickDetect(resumeText, window);

            //Close by clicking quit
            if (!buttonClickDetect(quitText, window)) {
                window.close();
            }
           

        }

        if (playerLives == 0) {

            //Fade into black
            screenshot.create(window.getSize().x, window.getSize().y);
            screenshot.update(window);

            pauseScreen.setTexture(screenshot);
            pauseScreen.setColor(grey);

            //Set new texts
            quitText.setPosition(window.getSize().x / 2 - quitText.getGlobalBounds().width / 2, window.getSize().y - 100);

            resumeText.setString("Restart");
            resumeText.setPosition(window.getSize().x / 2 - resumeText.getGlobalBounds().width / 2, window.getSize().y / 2 - resumeText.getGlobalBounds().height);

            gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2, 45);

            scoreText.setPosition(window.getSize().x / 2 - scoreText.getGlobalBounds().width / 2, 150);
            scoreNum.setPosition(window.getSize().x / 2 - scoreNum.getGlobalBounds().width / 2, 200);

            //Display
            window.draw(pauseScreen);

            window.draw(scoreNum);
            window.draw(scoreText);
            window.draw(quitText);
            window.draw(resumeText);
            window.draw(gameOverText);

            window.display();

            //Close by clicking quit
            if (!buttonClickDetect(quitText, window)) {
                window.close();
            }

            if (!buttonClickDetect(resumeText, window)) {
                resumeText.setString("Resume");
                scoreText.setPosition(window.getSize().x / 2 - scoreText.getGlobalBounds().width / 2 + 10, 0);
                scoreNum.setPosition(window.getSize().x / 2 - scoreNum.getGlobalBounds().width / 2 + scoreText.getGlobalBounds().width, 0);
                difficulty = 1;
                scorecheck = 10;
                playerLives = 3;
                score = 0;
                cucumber.updateSpeed(1);

            }
        }
    }

    return 0;
}