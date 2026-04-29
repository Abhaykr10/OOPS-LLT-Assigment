#include "Obstacle.h"

// Obstacle base class
Obstacle::Obstacle(float speed) : speed(speed), passedPlayer(false) {}

bool Obstacle::checkPassed(float playerX) {
    if (!passedPlayer && getGlobalBounds().left + getGlobalBounds().width < playerX) {
        passedPlayer = true;
        return true;
    }
    return false;
}

// Spike
Spike::Spike(float startX, float groundLevel, float speed) : Obstacle(speed) {
    triangle.setPointCount(3);
    triangle.setPoint(0, sf::Vector2f(20.0f, 0.0f));
    triangle.setPoint(1, sf::Vector2f(40.0f, 40.0f));
    triangle.setPoint(2, sf::Vector2f(0.0f, 40.0f));
    triangle.setFillColor(sf::Color::Red);
    triangle.setPosition(startX, groundLevel + 50.0f - 40.0f);
}

void Spike::update(float deltaTime) { triangle.move(-speed * deltaTime, 0); }
void Spike::draw(sf::RenderWindow& window) const { window.draw(triangle); }
sf::FloatRect Spike::getGlobalBounds() const { return triangle.getGlobalBounds(); }
bool Spike::isOffScreen() const { return triangle.getPosition().x + triangle.getGlobalBounds().width < 0; }

// Boulder
Boulder::Boulder(float startX, float groundLevel, float speed) : Obstacle(speed) {
    circle.setRadius(20.0f);
    circle.setOrigin(20.0f, 20.0f);
    circle.setFillColor(sf::Color(139, 69, 19));
    circle.setPosition(startX + 20.0f, groundLevel + 50.0f - 20.0f);
}

void Boulder::update(float deltaTime) {
    circle.move(-speed * deltaTime, 0);
    circle.rotate(-speed * deltaTime * 3.0f); 
}
void Boulder::draw(sf::RenderWindow& window) const { window.draw(circle); }
sf::FloatRect Boulder::getGlobalBounds() const { return circle.getGlobalBounds(); }
bool Boulder::isOffScreen() const { return circle.getPosition().x + circle.getGlobalBounds().width < 0; }

// Crusher
Crusher::Crusher(float startX, float groundLevel, float speed) : Obstacle(speed) {
    rect.setSize(sf::Vector2f(40.0f, 150.0f));
    rect.setFillColor(sf::Color(105, 105, 105));
    rect.setPosition(startX, groundLevel + 50.0f - 150.0f);
}

void Crusher::update(float deltaTime) {
    rect.move(-speed * deltaTime, 0);
}
void Crusher::draw(sf::RenderWindow& window) const { window.draw(rect); }
sf::FloatRect Crusher::getGlobalBounds() const { return rect.getGlobalBounds(); }
bool Crusher::isOffScreen() const { return rect.getPosition().x + rect.getGlobalBounds().width < 0; }
