#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <cmath>

class Player : public Entity {
private:
    float velocityY;
    float gravity;
    float jumpForce;
    bool isGrounded;
    float groundLevel;

    sf::RectangleShape hitbox;
    
    sf::CircleShape head;
    sf::RectangleShape body;
    sf::RectangleShape leftArm;
    sf::RectangleShape rightArm;
    sf::RectangleShape leftLeg;
    sf::RectangleShape rightLeg;
    float runAnimTimer;
    int jumpCount;

public:
    Player(float startX, float startY, sf::Color stickColor);

    void handleInput(sf::Keyboard::Key key);
    void jump();
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;
    sf::FloatRect getGlobalBounds() const override;
};

#endif // PLAYER_H
