#include "Game.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

// Star implementation
Star::Star(float x, float y, float speed) : speed(speed) {
    dot.setRadius((rand() % 15 + 10) / 10.0f);
    dot.setPosition(x, y);
    dot.setFillColor(sf::Color(255, 255, 255, rand() % 155 + 100));
}
void Star::update(float deltaTime) { dot.move(-speed * deltaTime, 0); }
void Star::draw(sf::RenderWindow& window) const { window.draw(dot); }
bool Star::isOffScreen() const { return dot.getPosition().x < -10.0f; }

// Mountain implementation
Mountain::Mountain(float startX, float groundY, float speed) : speed(speed) {
    float width = (rand() % 200) + 150.0f;
    float height = (rand() % 150) + 100.0f;
    
    peak.setPointCount(3);
    peak.setPoint(0, sf::Vector2f(0.0f, height));
    peak.setPoint(1, sf::Vector2f(width / 2.0f, 0.0f));
    peak.setPoint(2, sf::Vector2f(width, height));
    
    peak.setFillColor(sf::Color(40, 40, 55));
    peak.setPosition(startX, groundY + 50.0f - height);
}
void Mountain::update(float deltaTime) { peak.move(-speed * deltaTime, 0); }
void Mountain::draw(sf::RenderWindow& window) const { window.draw(peak); }
bool Mountain::isOffScreen() const { return peak.getPosition().x + peak.getGlobalBounds().width < 0; }

// Game implementation
Game::Game() 
    : window(sf::VideoMode(800, 600), "Stickman Survivor"), 
      spawnTimer(0.0f), 
      spawnInterval(1.5f),
      currentState(GameState::Menu),
      score(0),
      highScore(0),
      coins(0),
      playerColor(sf::Color::Green),
      ownsBlue(false),
      ownsPurple(false) {
    
    window.setFramerateLimit(60);
    std::random_device rd;
    rng.seed(rd());
    
    initUI();
    loadGame();
    resetGame();

    for (int i = 0; i < 100; i++) {
        float x = (rand() % 800);
        float y = (rand() % (int)groundY);
        stars.push_back(Star(x, y, (rand() % 20) + 10.0f));
    }
    for (int i = 0; i < 5; i++) {
        float x = (rand() % 1000) - 200.0f;
        mountains.push_back(Mountain(x, groundY, (rand() % 30) + 40.0f));
    }
}

void Game::initUI() {
    if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Courier New.ttf")) {
            if (!font.loadFromFile("/System/Library/Fonts/SFNS.ttf")) {
                std::cerr << "Warning: Could not load any macOS system fonts.\n";
            }
        }
    }

    titleText.setFont(font);
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);
    
    instructionText.setFont(font);
    instructionText.setCharacterSize(30);
    instructionText.setFillColor(sf::Color::Yellow);

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20.0f, 20.0f);

    storeText.setFont(font);
    storeText.setCharacterSize(28);
    storeText.setFillColor(sf::Color::White);
}

void Game::resetGame() {
    gameEntities.clear();
    player = std::make_unique<Player>(100.0f, groundY, playerColor);
    
    spawnTimer = 0.0f;
    score = 0;
}

void Game::spawnEntity() {
    float baseSpeed = 250.0f + (score * 5.0f); 
    float maxSpeed = 450.0f + (score * 5.0f);
    
    std::uniform_real_distribution<float> speedDist(baseSpeed, maxSpeed);
    std::uniform_int_distribution<int> typeDist(0, 10);
    
    float speed = speedDist(rng);
    int type = typeDist(rng);
    
    if (type < 3) {
        gameEntities.push_back(std::make_unique<Spike>(screenWidth, groundY, speed));
    } else if (type < 6) {
        gameEntities.push_back(std::make_unique<Boulder>(screenWidth, groundY, speed));
    } else if (type < 8) {
        gameEntities.push_back(std::make_unique<Crusher>(screenWidth, groundY, speed));
    } else {
        std::uniform_real_distribution<float> yDist(groundY - 120.0f, groundY - 20.0f);
        gameEntities.push_back(std::make_unique<Coin>(screenWidth, yDist(rng), speed));
    }
}

void Game::loadGame() {
    std::ifstream file("save.txt");
    if (file.is_open()) {
        file >> highScore >> coins;
        int b, p;
        file >> b >> p;
        ownsBlue = (b == 1);
        ownsPurple = (p == 1);
        int r, g, bl;
        if (file >> r >> g >> bl)
            playerColor = sf::Color(r, g, bl);
    }
}

void Game::saveGame() {
    std::ofstream file("save.txt");
    if (file.is_open()) {
        file << highScore << "\n" << coins << "\n" 
             << (ownsBlue ? 1 : 0) << "\n" << (ownsPurple ? 1 : 0) << "\n" 
             << (int)playerColor.r << " " << (int)playerColor.g << " " << (int)playerColor.b << "\n";
    }
}

void Game::buyOrEquip(bool& owned, sf::Color color, int cost) {
    if (owned) {
        playerColor = color;
    } else if (coins >= cost) {
        coins -= cost;
        owned = true;
        playerColor = color;
    }
    saveGame();
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    auto isStartKey = [](sf::Keyboard::Key k) {
        return k == sf::Keyboard::Space || k == sf::Keyboard::W || k == sf::Keyboard::Up || k == sf::Keyboard::Enter;
    };

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type != sf::Event::KeyPressed) continue;
        auto key = event.key.code;

        if (currentState == GameState::Menu) {
            if (isStartKey(key))      { resetGame(); currentState = GameState::Playing; }
            else if (key == sf::Keyboard::S) currentState = GameState::Store;
        } 
        else if (currentState == GameState::GameOver) {
            if (isStartKey(key))      { resetGame(); currentState = GameState::Playing; }
            else if (key == sf::Keyboard::M || key == sf::Keyboard::Escape) currentState = GameState::Menu;
        }
        else if (currentState == GameState::Store) {
            if (key == sf::Keyboard::Escape || key == sf::Keyboard::M) currentState = GameState::Menu;
            else if (key == sf::Keyboard::Num1) { playerColor = sf::Color::Green; saveGame(); }
            else if (key == sf::Keyboard::Num2) buyOrEquip(ownsBlue, sf::Color::Blue, 10);
            else if (key == sf::Keyboard::Num3) buyOrEquip(ownsPurple, sf::Color::Magenta, 20);
        }
        else if (currentState == GameState::Playing) {
            player->handleInput(key);
        }
    }
}

void Game::update() {
    float deltaTime = clock.restart().asSeconds();
    
    if (rand() % 100 < 5) stars.push_back(Star(810.0f, (rand() % (int)groundY), (rand() % 20) + 10.0f));
    if (rand() % 1000 < 5) mountains.push_back(Mountain(810.0f, groundY, (rand() % 30) + 40.0f));

    for (auto it = stars.begin(); it != stars.end();) {
        it->update(deltaTime);
        if (it->isOffScreen()) it = stars.erase(it);
        else ++it;
    }
    for (auto it = mountains.begin(); it != mountains.end();) {
        it->update(deltaTime);
        if (it->isOffScreen()) it = mountains.erase(it);
        else ++it;
    }

    if (currentState != GameState::Playing) return;

    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval) {
        spawnEntity();
        spawnTimer = 0.0f;
        
        std::uniform_real_distribution<float> intervalDist(1.0f, 2.5f);
        spawnInterval = intervalDist(rng);
    }

    player->update(deltaTime);

    auto it = gameEntities.begin();
    while (it != gameEntities.end()) {
        (*it)->update(deltaTime);
        bool removeEntity = false;
        
        Obstacle* obs = dynamic_cast<Obstacle*>(it->get());
        if (obs) {
            if (obs->isOffScreen()) {
                removeEntity = true;
            }
            else if (player->getGlobalBounds().intersects(obs->getGlobalBounds())) {
                currentState = GameState::GameOver;
                return;
            }
            else if (obs->checkPassed(player->getGlobalBounds().left)) {
                score++;
                if (score > highScore) { highScore = score; saveGame(); }
            }
        }

        Coin* coin = dynamic_cast<Coin*>(it->get());
        if (coin) {
            if (coin->isOffScreen() || coin->isCollected()) {
                removeEntity = true;
            } else if (player->getGlobalBounds().intersects(coin->getGlobalBounds())) {
                coin->collect();
                coins++;
                saveGame();
                removeEntity = true;
            }
        }
        
        if (removeEntity) {
            it = gameEntities.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::centerText(sf::Text& text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

void Game::render() {
    window.clear(sf::Color(20, 20, 30));

    for (const auto& s : stars) s.draw(window);
    for (const auto& m : mountains) m.draw(window);

    sf::RectangleShape groundLine(sf::Vector2f(screenWidth, 5.0f));
    groundLine.setPosition(0, groundY + 50.0f); 
    groundLine.setFillColor(sf::Color::White);
    window.draw(groundLine);

    if (currentState == GameState::Playing || currentState == GameState::GameOver) {
        player->draw(window);
        for (const auto& ent : gameEntities) {
            ent->draw(window);
        }
        
        scoreText.setString("Score: " + std::to_string(score) + " | Coins: " + std::to_string(coins) + "\nHigh Score: " + std::to_string(highScore));
        window.draw(scoreText);
    }

    if (currentState == GameState::Menu) {
        titleText.setString("STICKMAN SURVIVOR");
        titleText.setFillColor(sf::Color::White);
        centerText(titleText, screenWidth / 2.0f, 150.0f);
        
        instructionText.setString("Press SPACE to Start\nPress S to enter STORE");
        centerText(instructionText, screenWidth / 2.0f, 300.0f);
        
        window.draw(titleText);
        window.draw(instructionText);
    }
    else if (currentState == GameState::Store) {
        titleText.setString("CHARACTER STORE");
        titleText.setFillColor(sf::Color::Yellow);
        centerText(titleText, screenWidth / 2.0f, 100.0f);
        
        storeText.setString(
            "Your Coins: " + std::to_string(coins) + "\n\n" +
            "1: Green Stickman (Free/Default)\n" +
            "2: Blue Stickman (10 Coins) " + (ownsBlue ? "[OWNED]" : "") + "\n" +
            "3: Purple Stickman (20 Coins) " + (ownsPurple ? "[OWNED]" : "") + "\n\n" +
            "Press the number key to buy/equip!\n" +
            "Press ESC to return to Menu"
        );
        centerText(storeText, screenWidth / 2.0f, 300.0f);

        window.draw(titleText);
        window.draw(storeText);
    }
    else if (currentState == GameState::GameOver) {
        titleText.setString("GAME OVER");
        titleText.setFillColor(sf::Color::Red);
        centerText(titleText, screenWidth / 2.0f, 150.0f);

        instructionText.setString("Press SPACE to Restart\nPress M to return to Menu");
        centerText(instructionText, screenWidth / 2.0f, 300.0f);

        window.draw(titleText);
        window.draw(instructionText);
    }

    window.display();
}
