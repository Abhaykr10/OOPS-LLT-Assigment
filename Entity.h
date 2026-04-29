#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

// Game States
enum class GameState { Menu, Playing, GameOver, Store };

// Abstract Base Class
class Entity {
public:
    virtual ~Entity() = default;
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual sf::FloatRect getGlobalBounds() const = 0;
};

#endif // ENTITY_H
