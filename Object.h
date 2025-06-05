#pragma once
#include <SFML/Graphics.hpp>
#include "ControlEng.h"
#include <variant>

const float PI = 3.14159265f;

sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

 class Object {
    public:
        virtual void draw(sf::RenderWindow& window) const = 0;
        virtual void save(std::ofstream& out) const = 0;
        virtual ~Object() {} // <--- ВАЖНО!
 };


struct Wall : public Object {

    bool visibleWall = false;
    sf::Vector2f wallX, wallY;
    int colorWall;
    sf::Color coloorWallq;

    Wall() : wallX(0, 0), wallY(0, 0), colorWall(1), coloorWallq(sf::Color::White) {}
    Wall(sf::Vector2f a, sf::Vector2f b, int colorWall = 1)
        : wallX(a), wallY(b), colorWall(colorWall), coloorWallq(getWallColor(colorWall)) {
    }

    void draw(sf::RenderWindow& win) const override {
        sf::Vertex line[] = {
            sf::Vertex(wallX, coloorWallq),
            sf::Vertex(wallY, coloorWallq)
        };
        win.draw(line, 2, sf::Lines);
    }

    void save(std::ofstream& out) const override {
        out << "Wall "
            << wallX.x << " " << wallX.y << " "
            << wallY.x << " " << wallY.y << " "
            << colorWall << "\n";
    }

    sf::Color getWallColor(int index) {
        switch (index) {
        case 1: return sf::Color::White;
        case 2: return sf::Color::Red;
        case 3: return sf::Color::Green;
        case 4: return sf::Color::Blue;
        case 5: return sf::Color::Yellow;
        case 6: return sf::Color::Magenta;
        case 7: return sf::Color::Cyan;
        case 8: return sf::Color(255, 128, 0);
        case 9: return sf::Color(128, 0, 255);
        default: return sf::Color::White;
        }
    }
};


struct Player : public Object {
public:

    bool isCreatePlayer = false;

    sf::Texture playerTexture;
    sf::Sprite playerSprite;

    sf::Vector2f posPlayer;
    float angle = 0; // направление взгляда
    float fov = 90;  // поле зрения
    float speed = 5.0f;
    float x, y;
    double w,h;

    

    Player(float x, float y): x(x), y(y) {
        
        posPlayer = { x, y };
        playerSprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
        
    }

    void setTextureSize(const sf::RenderWindow& window) {

        sf::Vector2u winSize = window.getSize();
        sf::Vector2u texSize = playerTexture.getSize();

        float scaleX = static_cast<float>(winSize.x) / texSize.x;
        float scaleY = static_cast<float>(winSize.y) / texSize.y;
    }
    void update() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            angle -= 2;
            std::cout << " andge = " << angle << std::endl; 
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            angle += 2;
            std::cout << " angle = " << angle << std::endl;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            posPlayer.x += std::cos(angle * PI / 180) * speed;
            posPlayer.y += std::sin(angle * PI / 180) * speed;
            std::cout << "Px " << posPlayer.x << " Py " << posPlayer.y << std::endl;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            posPlayer.x -= std::cos(angle * PI / 180) * speed;
            posPlayer.y -= std::sin(angle * PI / 180) * speed;
            std::cout << "Px " << posPlayer.x << " Py " << posPlayer.y << std::endl;
        }
        
        playerSprite.setPosition(posPlayer.x - 50, posPlayer.y - 90);
        
       
    }

    void castRays(sf::RenderWindow& win,  std::vector<Wall>& walls) { // сделать нормальное отсечение стен
        int numRays = 120;
        float step = fov / numRays;

        for (auto& wall : walls) {
            wall.visibleWall = false;
        }

        for (int i = 0; i < numRays; i++) {

            float rayAngle = (angle - fov / 2.0f) + i * step;
            float rayX = std::cos(rayAngle * PI / 180);
            float rayY = std::sin(rayAngle * PI / 180);
            float minDist = 500;
            sf::Vector2f hitPoint = posPlayer + sf::Vector2f(rayX, rayY) * minDist;

            for (auto& wall : walls) {
                sf::Vector2f p = posPlayer;
                sf::Vector2f d = { rayX, rayY };
                sf::Vector2f v1 = wall.wallX;
                sf::Vector2f v2 = wall.wallY;
                sf::Vector2f v = v2 - v1;

                float denom = d.x * v.y - d.y * v.x;
                if (std::abs(denom) < 0.0001f) continue;

                float t = ((v1.x - p.x) * v.y - (v1.y - p.y) * v.x) / denom;
                float u = ((v1.x - p.x) * d.y - (v1.y - p.y) * d.x) / denom;

                if (t > 0 && u >= 0 && u <= 1) {
                    sf::Vector2f intersect = p + t * d;
                    float dist = std::hypot(intersect.x - posPlayer.x, intersect.y - posPlayer.y);
                    if (dist < minDist) {
                        minDist = dist;
                        hitPoint = intersect;
                        wall.visibleWall = true;
                    }   
                  }
            }

            /*sf::Vertex ray[] = {
                sf::Vertex(posPlayer, sf::Color::White),
                sf::Vertex(hitPoint, sf::Color::White)
            };
           
            win.draw(ray, 2, sf::Lines);*/
        }
    }

    void draw(sf::RenderWindow& win) const override {
        sf::CircleShape playerShape(5);
        playerShape.setFillColor(sf::Color::Green);
        playerShape.setOrigin(5, 5);
        playerShape.setPosition(posPlayer.x, posPlayer.y);
        win.draw(playerShape);
    }

    void save(std::ofstream& out) const override {
        out << "Player " << posPlayer.x << " " << posPlayer.y << " " << angle << "\n";
    }

private:
    sf::Texture texture;
    sf::Sprite sprite;
};


struct Enemy{
public:
    Player* playerT;

    sf::Texture enemyTexture;
    sf::Sprite enemySprite;
    sf::Vector2f posEnemy;
    
    int ghostActivity = 0;

    float x, y;
    float speedEnemy = 1.f;
    
    bool isEnemyCreated = false;
    bool isAttack = false;
    bool playerIsSeeEnemy = false;
    bool EnemySee = false;
    
    Enemy(float x, float y) : x(x), y(y) {

      posEnemy = { x, y };
      enemySprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
      enemySprite.setColor(sf::Color::Red);
    }
    
    void update() {
        
            posEnemy.x += speedEnemy;
           // posEnemy.x -=  speedEnemy;
            posEnemy.y +=  speedEnemy;
            posEnemy.y -=  speedEnemy;
            enemySprite.setPosition(posEnemy.x - 50, posEnemy.y - 90);
        
    }

    void draw() {

    }

};

struct Background {
    sf::Texture backTexture;
    sf::Sprite backSprite;
    sf::Vector2f pos;
    float x, y;

    Background(float x, float y) : x(x), y(y) {
        pos = { x, y };
    }

    void setTextureAndScale(const std::string& path, const sf::RenderWindow& window) {
        if (backTexture.loadFromFile(path)) {
            backSprite.setTexture(backTexture);

            sf::Vector2u winSize = window.getSize();
            sf::Vector2u texSize = backTexture.getSize();

            float scaleX = static_cast<float>(winSize.x) / texSize.x;
            float scaleY = static_cast<float>(winSize.y) / texSize.y;

            backSprite.setScale(scaleX, scaleY);
        }
        else {
            std::cerr << "Failed to load background texture\n";
        }
    }
     
};

struct Text {
    Text(const std::string& path, const sf::RenderWindow& window) {
        font.loadFromFile(path); 
        sf::Text text(" ", font, 20);
        text.setFillColor(sf::Color::Red);
        text.setStyle(sf::Text::Bold);
    }

    sf::Font font;
};

class Light {
public:

    Light(float x, float y) : x(x), y(y) {

        pos = { x, y };
    }

    sf::Vector2f start;
    sf::Vector2f pos;
    float angle = 0; // направление взгляда
    float fov = 360;  // поле зрения
    float speed = 5.0f;
    float x, y;
    bool isDraw = false;

    void TestLight(sf::RenderWindow& win, const std::vector<Wall>& walls) { // сделать нормальное отсечение стен
        int numRays = 720;
        float step = fov / numRays;

        for (int i = 0; i < numRays; i++) {
            float rayAngle = (angle - fov / 2.0f) + i * step;
            float rayX = std::cos(rayAngle * PI / 180);
            float rayY = std::sin(rayAngle * PI / 180);
            float minDist = 200;
            sf::Vector2f hitPoint = pos + sf::Vector2f(rayX, rayY) * minDist;

            for (const auto& wall : walls) {
                sf::Vector2f p = pos;
                sf::Vector2f d = { rayX, rayY };
                sf::Vector2f v1 = wall.wallX;
                sf::Vector2f v2 = wall.wallY;
                sf::Vector2f v = v2 - v1;

                float denom = d.x * v.y - d.y * v.x;
                if (std::abs(denom) < 0.0001f) continue;

                float t = ((v1.x - p.x) * v.y - (v1.y - p.y) * v.x) / denom;
                float u = ((v1.x - p.x) * d.y - (v1.y - p.y) * d.x) / denom;

                if (t > 0 && u >= 0 && u <= 1) {
                    sf::Vector2f intersect = p + t * d;
                    float dist = std::hypot(intersect.x - pos.x, intersect.y - pos.y);
                    if (dist < minDist) {
                        minDist = dist;
                        hitPoint = intersect;

                    }

                }
            }

            sf::Vertex ray[] = {
                sf::Vertex(pos, sf::Color::White),
                sf::Vertex(hitPoint, sf::Color::White)
            };

            win.draw(ray, 2, sf::Lines);
        }
    }

    void draw(sf::RenderWindow& win) {
        sf::CircleShape playerShape(5);
        playerShape.setFillColor(sf::Color::Green);
        playerShape.setOrigin(10, 15);
        playerShape.setPosition(pos);
        win.draw(playerShape);
    }

    

    void drawDelay() {

     }
};
