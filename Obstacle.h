#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Entity.h"

class Obstacle : public Entity {
protected:
    float speed;
    bool passedPlayer;
public:
    Obstacle(float speed);
    
    virtual bool isOffScreen() const = 0;
    
    bool checkPassed(float playerX);
};

class Spike : public Obstacle {
private:
    sf::ConvexShape triangle;
public:
    Spike(float startX, float groundLevel, float speed);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;
    sf::FloatRect getGlobalBounds() const override;
    bool isOffScreen() const override;
};

class Boulder : public Obstacle {
private:
    sf::CircleShape circle;
public:
    Boulder(float startX, float groundLevel, float speed);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;
    sf::FloatRect getGlobalBounds() const override;
    bool isOffScreen() const override;
};

class Crusher : public Obstacle {
private:
    sf::RectangleShape rect;
public:
    Crusher(float startX, float groundLevel, float speed);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;
    sf::FloatRect getGlobalBounds() const override;
    bool isOffScreen() const override;
};

#endif // OBSTACLE_H
