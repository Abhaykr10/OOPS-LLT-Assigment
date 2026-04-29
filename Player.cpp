#include "Player.h"

Player::Player(float startX, float startY, sf::Color stickColor) 
    : velocityY(0.0f), gravity(1500.0f), jumpForce(-650.0f), isGrounded(false), groundLevel(startY), runAnimTimer(0.0f), jumpCount(0) {
    
    hitbox.setSize(sf::Vector2f(50.0f, 50.0f));
    hitbox.setFillColor(sf::Color::Transparent); 
    hitbox.setPosition(startX, startY);

    head.setRadius(8.0f);
    head.setFillColor(stickColor);
    head.setOrigin(8.0f, 8.0f);

    body.setSize(sf::Vector2f(4.0f, 20.0f));
    body.setFillColor(stickColor);
    body.setOrigin(2.0f, 0.0f);

    leftArm.setSize(sf::Vector2f(4.0f, 16.0f));
    leftArm.setFillColor(stickColor);
    leftArm.setOrigin(2.0f, 2.0f); 
    
    rightArm.setSize(sf::Vector2f(4.0f, 16.0f));
    rightArm.setFillColor(stickColor);
    rightArm.setOrigin(2.0f, 2.0f);

    leftLeg.setSize(sf::Vector2f(4.0f, 18.0f));
    leftLeg.setFillColor(stickColor);
    leftLeg.setOrigin(2.0f, 2.0f);

    rightLeg.setSize(sf::Vector2f(4.0f, 18.0f));
    rightLeg.setFillColor(stickColor);
    rightLeg.setOrigin(2.0f, 2.0f);
}

void Player::handleInput(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Space || key == sf::Keyboard::W || key == sf::Keyboard::Up) {
        jump();
    }
}

void Player::jump() {
    if (jumpCount < 2) {
        velocityY = jumpForce;
        isGrounded = false;
        jumpCount++;
    }
}

void Player::update(float deltaTime) {
    if (!isGrounded) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            velocityY += gravity * 3.5f * deltaTime;
        } else {
            velocityY += gravity * deltaTime;
        }
    }

    float moveX = 0.0f;
    float moveSpeed = 300.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        moveX = -moveSpeed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        moveX = moveSpeed * deltaTime;
    }

    hitbox.move(moveX, velocityY * deltaTime);

    if (hitbox.getPosition().x < 0.0f) hitbox.setPosition(0.0f, hitbox.getPosition().y);
    if (hitbox.getPosition().x + 50.0f > 800.0f) hitbox.setPosition(800.0f - 50.0f, hitbox.getPosition().y);

    if (hitbox.getPosition().y >= groundLevel) {
        hitbox.setPosition(hitbox.getPosition().x, groundLevel);
        velocityY = 0.0f;
        isGrounded = true;
        jumpCount = 0;
    } else {
        isGrounded = false;
    }

    sf::Vector2f pos = hitbox.getPosition();
    float cx = pos.x + 25.0f;
    float cy = pos.y;

    head.setPosition(cx, cy + 10.0f);
    body.setPosition(cx, cy + 18.0f);
    leftArm.setPosition(cx, cy + 20.0f);
    rightArm.setPosition(cx, cy + 20.0f);
    leftLeg.setPosition(cx, cy + 36.0f);
    rightLeg.setPosition(cx, cy + 36.0f);

    if (isGrounded) {
        if (moveX != 0.0f) {
            runAnimTimer += deltaTime * 20.0f;
        } else {
            runAnimTimer += deltaTime * 10.0f; 
        }
        float swing = std::sin(runAnimTimer) * 45.0f; 
        
        leftLeg.setRotation(swing);
        rightLeg.setRotation(-swing);
        leftArm.setRotation(-swing);
        rightArm.setRotation(swing);
    } else {
        leftArm.setRotation(-150.0f); 
        rightArm.setRotation(150.0f);
        leftLeg.setRotation(30.0f);  
        rightLeg.setRotation(-45.0f); 
    }
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(leftArm);
    window.draw(leftLeg);
    window.draw(body);
    window.draw(head);
    window.draw(rightLeg);
    window.draw(rightArm);
}

sf::FloatRect Player::getGlobalBounds() const {
    return hitbox.getGlobalBounds();
}
