#ifndef GAME_H
#define GAME_H

#include "Entity.h"
#include "Player.h"
#include "Obstacle.h"
#include "Coin.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <memory>
#include <random>
#include <string>

class Star {
public:
    sf::CircleShape dot;
    float speed;
    
    Star(float x, float y, float speed);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool isOffScreen() const;
};

class Mountain {
public:
    sf::ConvexShape peak;
    float speed;
    
    Mountain(float startX, float groundY, float speed);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool isOffScreen() const;
};

class Game {
private:
    sf::RenderWindow window;
    
    std::vector<std::unique_ptr<Entity>> gameEntities;
    std::vector<Star> stars;
    std::vector<Mountain> mountains;
    std::unique_ptr<Player> player;
    
    sf::Clock clock;
    float spawnTimer;
    float spawnInterval;
    
    std::mt19937 rng;
    
    const float groundY = 450.0f;
    const float screenWidth = 800.0f;

    GameState currentState;
    int score;
    int highScore;
    int coins;

    sf::Color playerColor;
    bool ownsBlue;
    bool ownsPurple;

    sf::Font font;
    sf::Text titleText;
    sf::Text instructionText;
    sf::Text scoreText;
    sf::Text storeText;

    void initUI();
    void resetGame();
    void spawnEntity();
    void processEvents();
    void update();
    void centerText(sf::Text& text, float x, float y);
    void render();
    void buyOrEquip(bool& owned, sf::Color color, int cost);

public:
    Game();
    void loadGame();
    void saveGame();
    void run();
};

#endif // GAME_H
