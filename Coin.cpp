#include "Coin.h"

Coin::Coin(float startX, float yPos, float speed) : speed(speed), collected(false) {
    circle.setRadius(12.0f);
    circle.setOrigin(12.0f, 12.0f);
    circle.setFillColor(sf::Color::Yellow);
    circle.setOutlineColor(sf::Color(255, 215, 0));
    circle.setOutlineThickness(2.0f);
    circle.setPosition(startX, yPos);
}

void Coin::update(float deltaTime) {
    circle.move(-speed * deltaTime, 0);
}

void Coin::draw(sf::RenderWindow& window) const { window.draw(circle); }
sf::FloatRect Coin::getGlobalBounds() const { return circle.getGlobalBounds(); }
bool Coin::isOffScreen() const { return circle.getPosition().x + circle.getGlobalBounds().width < 0; }
bool Coin::isCollected() const { return collected; }
void Coin::collect() { collected = true; }
