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

    bool isCreatePlayer = false; bool isPlayerWalk = false;
    bool PlayerIsDetected = false; 

    sf::Texture playerTexture;sf::Sprite playerSprite;
    sf::Vector2f posPlayer;

    float angle = 0; float fov = 90; 
    float speed = 5.0f; float playerRadius = 15.f;
    float x, y; double w, h;


    Player(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {

        posPlayer = { x, y };
        playerSprite.setTextureRect(sf::IntRect(0, 0, w, h));

    }

    void setTextureSize(const sf::RenderWindow& window) {

        sf::Vector2u winSize = window.getSize();
        sf::Vector2u texSize = playerTexture.getSize();

        float scaleX = static_cast<float>(winSize.x) / texSize.x;
        float scaleY = static_cast<float>(winSize.y) / texSize.y;
    }
    void update(float time) {
        isPlayerWalk = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            angle -= 2;
            if (angle < 0) angle += 360;
            std::cout << "angle = " << angle << std::endl;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            angle += 2;
            if (angle >= 360) angle -= 360;
            std::cout << "angle = " << angle << std::endl;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            posPlayer.x += std::cos(angle * PI / 180) * speed;
            posPlayer.y += std::sin(angle * PI / 180) * speed;
            isPlayerWalk = true;
            std::cout << "Px " << posPlayer.x << " Py " << posPlayer.y << std::endl;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            posPlayer.x -= std::cos(angle * PI / 180) * speed;
            posPlayer.y -= std::sin(angle * PI / 180) * speed;
            isPlayerWalk = true;
            std::cout << "Px " << posPlayer.x << " Py " << posPlayer.y << std::endl;
        }
        if (isPlayerWalk) {
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
        }
        else {
            CurrentFrame = 0;
        }
        if (angle >= 45 && angle < 135) {
            playerSprite.setTextureRect(sf::IntRect(96 * int(CurrentFrame), 0, 96, 96));
        }
        else if (angle >= 135 && angle < 225) {
            playerSprite.setTextureRect(sf::IntRect(96 * int(CurrentFrame), 130, 100, 100));
        }
        else if (angle >= 225 && angle < 315) {
            playerSprite.setTextureRect(sf::IntRect(96 * int(CurrentFrame), 307, 96, 96));
        }
        else {
            playerSprite.setTextureRect(sf::IntRect(96 * int(CurrentFrame), 200, 96, 96));
        }
        playerSprite.setPosition(posPlayer.x - 48, posPlayer.y - 48);
    }

    void castRays(sf::RenderWindow& win, std::vector<Wall>& walls) { // сделать нормальное отсечение стен
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

            sf::Vertex ray[] = {
                sf::Vertex(posPlayer, sf::Color::White),
                sf::Vertex(hitPoint, sf::Color::White)
            };

            win.draw(ray, 2, sf::Lines);
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
    float CurrentFrame = 0;

};


struct Enemy {
public:
    Player* playerT;

    sf::Texture enemyTexture; sf::Sprite enemySprite;
    sf::Vector2f posEnemy; sf::Vector2f dir;
    sf::Vector2f lastPlayerPosSee;

    int ghostActivity = 0;

    float x, y; float speedEnemy = 4.f;
    float fov = 90; float angle = 0; float length;
    float lookAngle = 0.f; float lookTimer = 0.f; 
    float maxLookTime = 120.f; float originalAngle = 0.f; 

    bool isEnemyWalk = false; bool isEnemyCreated = false;
    bool isAttack = false; bool playerIsSeeEnemy = false;
    bool enemySeePlayer = false; bool isLookingAround = false;
    bool lookDirection = true; bool isGoingToLastPos = false;
    bool searchPlayer = false;
    
    Enemy(float x, float y, Player* playerT) : x(x), y(y), playerT(playerT) {
        posEnemy = { x, y };
        dir = playerT->posPlayer - posEnemy;
        enemySprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
        enemySprite.setColor(sf::Color::Red);
    }


    void castRays(sf::RenderWindow& win,std::vector<Wall>walls) {
        int numRays = 120;
        float step = fov / numRays;

        playerT->PlayerIsDetected = false;
        
        for (int i = 0; i < numRays; i++) {
            float rayAngle = (angle - fov / 2.0f) + i * step;
            float rayX = std::cos(rayAngle * PI / 180);
            float rayY = std::sin(rayAngle * PI / 180);
            sf::Vector2f rayDir = { rayX, rayY };

            float maxDist = 500.f;

            sf::Vector2f toPlayer = playerT->posPlayer - posEnemy;
            float proj = toPlayer.x * rayDir.x + toPlayer.y * rayDir.y; 

            bool playerHit = false;
            float distToPlayer = maxDist;

            if (proj > 0) { 
                sf::Vector2f closestPoint = posEnemy + rayDir * proj;
                float distToCircle = std::hypot(closestPoint.x - playerT->posPlayer.x, closestPoint.y - playerT->posPlayer.y);

                if (distToCircle <= playerT->playerRadius) {
                    float offset = std::sqrt(playerT->playerRadius * playerT->playerRadius - distToCircle * distToCircle);
                    distToPlayer = proj - offset; 
                    playerHit = true;
                }
            }

            float minDist = maxDist;
            sf::Vector2f hitPoint = posEnemy + rayDir * maxDist;

            for (auto& wall : walls) {
                sf::Vector2f p = posEnemy;
                sf::Vector2f d = rayDir;
                sf::Vector2f v1 = wall.wallX;
                sf::Vector2f v2 = wall.wallY;
                sf::Vector2f v = v2 - v1;

                float denom = d.x * v.y - d.y * v.x;
                if (std::abs(denom) < 0.0001f) continue;

                float t = ((v1.x - p.x) * v.y - (v1.y - p.y) * v.x) / denom;
                float u = ((v1.x - p.x) * d.y - (v1.y - p.y) * d.x) / denom;

                if (t > 0 && u >= 0 && u <= 1) {
                    sf::Vector2f intersect = p + t * d;
                    float dist = std::hypot(intersect.x - posEnemy.x, intersect.y - posEnemy.y);
                    if (dist < minDist) {
                        minDist = dist;
                        hitPoint = intersect;
                    }
                }
            }

            if (playerHit && distToPlayer < minDist) {
                enemySeePlayer = true;
                playerT->PlayerIsDetected = true;
                minDist = distToPlayer;
                hitPoint = posEnemy + rayDir * distToPlayer;
            }

            sf::Vertex ray[] = {
                sf::Vertex(posEnemy, sf::Color::White),
                sf::Vertex(hitPoint, sf::Color::White)
            };
            win.draw(ray, 2, sf::Lines);
        }
    }
    void runAfterPlayer() {
        if (enemySeePlayer && playerT->PlayerIsDetected) {
            
            dir = playerT->posPlayer - posEnemy;
            length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (length != 0) {
                dir.x /= length;
                dir.y /= length;
            }

            speedEnemy = 3.f;
            posEnemy += dir * speedEnemy;
            enemySprite.setPosition(posEnemy.x - 50, posEnemy.y - 50);
            angle = std::atan2(dir.y, dir.x) * 180 / PI;

            lastPlayerPosSee = playerT->posPlayer;

            isLookingAround = false;
        }

        else if (!enemySeePlayer && !isGoingToLastPos && !isLookingAround) {
            isGoingToLastPos = true;
        }

        if (isGoingToLastPos) {
            dir = lastPlayerPosSee - posEnemy;

            length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (length > 2.f) {
                dir.x /= length;
                dir.y /= length;

                speedEnemy = 1.f;
                posEnemy += dir * speedEnemy;
                enemySprite.setPosition(posEnemy.x - 50, posEnemy.y - 50);

                angle = std::atan2(dir.y, dir.x) * 180 / PI;
            }
            else {
                isGoingToLastPos = false;
                isLookingAround = true;
                lookTimer = 0.f;
                originalAngle = angle;
            }
        }

        if (isLookingAround) {
            lookTimer += 1.f;
            if (static_cast<int>(lookTimer) % 60 < 30) {
                lookAngle = originalAngle + 30.f;
            }
            else {
                lookAngle = originalAngle - 30.f;
            }
            angle = lookAngle;
            if (lookTimer > maxLookTime) {
                angle = originalAngle;
                isLookingAround = false;
                update();
            }
        }
    }

    void update() {
        runAfterPlayer();
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
