#ifndef COIN_H
#define COIN_H

#include "Entity.h"

class Coin : public Entity {
private:
    sf::CircleShape circle;
    float speed;
    bool collected;
public:
    Coin(float startX, float yPos, float speed);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;
    sf::FloatRect getGlobalBounds() const override;
    bool isOffScreen() const;
    bool isCollected() const;
    void collect();
};

#endif // COIN_H
