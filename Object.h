#pragma once
#include <SFML/Graphics.hpp>
#include "ControlEng.h"
#include <variant>
#include <cmath>
#include <SFML/Audio.hpp>
#include "WhereAnySounds.h"
//#include "Sounds.h"

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

    //Sounds soundShoot("shoot.ogg");
    //Sounds soundDeath{ "DeathPlayer.mp3" };
    
   
    sf::Sound sound;
    
    //Sounds soundReload("");

    sf::Texture playerTexture;sf::Sprite playerSprite;
    sf::Vector2f posPlayer;
    // сделать звуки всякой хуйни тестово и сделать звуки на определенную дистанцию
    // игрок бежит - звук > идет < стоит - 
    // игрок хромает (хп <= 50) звук +- бежит так же кровавые сопли
    // 

    

    bool isCreatePlayer = false; bool isPlayerWalk = false;
    bool PlayerIsDetected = false; bool isPlayerAttack = false;
    bool shootgunInInventory = false; bool shootgun = true;
    

    int playerHP = 100;

    float angle = 0; float fov = 90; 
    float speed = 5.0f; float playerRadius = 15.f;
    float x, y; double w, h;
    
    
    std::vector<sf::SoundBuffer> buffer;
    std::vector<int> playerInventory;

    Player(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {
       
        
        /*buffer.loadFromFile("DeathPlayer.ogg");
        sound.setBuffer(buffer);*/
        
        posPlayer = { x, y };
        playerSprite.setTextureRect(sf::IntRect(0, 0, w, h));
        playerInventory.resize(5);

    }

    void setTextureSize(const sf::RenderWindow& window) {

        sf::Vector2u winSize = window.getSize();
        sf::Vector2u texSize = playerTexture.getSize();

        float scaleX = static_cast<float>(winSize.x) / texSize.x;
        float scaleY = static_cast<float>(winSize.y) / texSize.y;
    }
    void update(float time) {
        playerMove_Anim(time);
        playerIsAttack();
       
        
    }

    void playerMove_Anim(float time) {
        isPlayerWalk = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            angle -= 2;
            if (angle < 0) angle += 360;
           // std::cout << "angle = " << angle << std::endl;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            angle += 2;
            if (angle >= 360) angle -= 360;
            //std::cout << "angle = " << angle << std::endl;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            posPlayer.x += std::cos(angle * PI / 180) * speed;
            posPlayer.y += std::sin(angle * PI / 180) * speed;
            isPlayerWalk = true;
            //std::cout << "Px " << posPlayer.x << " Py " << posPlayer.y << std::endl;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            posPlayer.x -= std::cos(angle * PI / 180) * speed;
            posPlayer.y -= std::sin(angle * PI / 180) * speed;
            isPlayerWalk = true;
            //std::cout << "Px " << posPlayer.x << " Py " << posPlayer.y << std::endl;
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

    void soundPlayer() {
        if (isPlayerWalk) {

        }
    }

    //void playerKeyboard() { // сюда свитч
    //    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
    //        if (shootgunInInventory) {
    //            playerSprite.setColor(sf::Color::Yellow);
    //        }
    //    }
    //    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
    //    }
    //    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
    //    }
    //    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
    //    }
    //    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
    //    }
    //}
    void playerIsAttack() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            isPlayerAttack = true;
            soundTest.play();
            playerSprite.setColor(sf::Color::Blue);
            //PlaysSound("SoundMusic/Sounds/DeathPlayer.ogg");
            
        }
        else playerSprite.setColor(sf::Color::White);
    }
    //void playerPickUpItem() {
    //    if (posPlayer == furn->posSquare) {
    //        shootgunInInventory = true; // сюда свитч сделать
    //        playerInventory.push_back(1);
    //    }
    //}

    void castRays(sf::RenderWindow& win, std::vector<Wall>& walls) { 
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
    float distantAttack = 5.f;

    bool isEnemyWalk = false; bool isEnemyCreated = false;
    bool isAttackPlayer = false; bool playerIsSeeEnemy = false;
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

            isGoingToLastPos = false;
            isLookingAround = false;

            dir = playerT->posPlayer - posEnemy;
            length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (length != 0) {
                dir.x /= length;
                dir.y /= length;
            }
            isEnemyWalk = true;
            speedEnemy = 3.f;
            posEnemy += dir * speedEnemy;

            enemySprite.setPosition(posEnemy.x - 50, posEnemy.y - 50);
            angle = std::atan2(dir.y, dir.x) * 180 / PI;

            lastPlayerPosSee = playerT->posPlayer;
        }
        else {

            if (!isGoingToLastPos && !isLookingAround) {
                isGoingToLastPos = true;
                isEnemyWalk = true;
            }

            if (isGoingToLastPos) {
                dir = lastPlayerPosSee - posEnemy;
                length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                if (length > 2.f) {
                    dir.x /= length;
                    dir.y /= length;
                    isEnemyWalk = true;

                    speedEnemy = 1.f;
                    posEnemy += dir * speedEnemy;
                    enemySprite.setPosition(posEnemy.x - 50, posEnemy.y - 50);
                    angle = std::atan2(dir.y, dir.x) * 180 / PI;
                }
                else {

                    isGoingToLastPos = false;
                    isEnemyWalk = false;

                    lookTimer = 0.f;
                    originalAngle = angle;
                    isLookingAround = true;
                }
            }
            
        }
    }

    void EnemyLookingAround() {
        if (isLookingAround) {
            lookTimer += 1.f;

            float offset = std::sin(lookTimer * 0.05f) * 30.f;
            angle = originalAngle + offset;

            if (lookTimer > maxLookTime) {
                angle = originalAngle;
                isLookingAround = false;
            }
        }
    }

   
    void animEnemy(float time) {
        if (isEnemyWalk) {
            if (angle < 0) angle += 360;
            //angle += 2;
            if (angle >= 360) angle -= 360;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            if (angle >= 45 && angle < 135) {
                enemySprite.setTextureRect(sf::IntRect(96 * int(CurrentFrame), 0, 96, 96));
            }
            else if (angle >= 135 && angle < 225) {
                enemySprite.setTextureRect(sf::IntRect(96 * int(CurrentFrame), 130, 100, 100));
            }
            else if (angle >= 225 && angle < 315) {
                enemySprite.setTextureRect(sf::IntRect(96 * int(CurrentFrame), 307, 96, 96));
            }
            else {
                enemySprite.setTextureRect(sf::IntRect(96 * int(CurrentFrame), 200, 96, 96));
            }
        }
        else { enemySprite.setTextureRect(sf::IntRect(96 * int(CurrentFrame), 0, 96, 96)); }
    }

    void update(float time) {
        runAfterPlayer();
        EnemyLookingAround();
        animEnemy(time);
    }
    void draw() {

    }
    private :
		float CurrentFrame = 0;
};

struct Furniture {
  
    sf::Vector2f posSquare;
    float x = 200; float y = 200;

    Furniture(float x, float y) : x(x), y(y) {
        posSquare = {x,y};
    }

    void draw(sf::RenderWindow& win) {
        sf::RectangleShape square(sf::Vector2f(50.f, 50.f));
        square.setOrigin(10, 15);
        square.setPosition(200,200);
        win.draw(square);
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



struct Floor {
    Floor(const std::string& file) {
        if (!texture.loadFromFile("Images/Textures/" + file)) {
            throw std::runtime_error("Не удалось загрузить текстуру: " + file);
        }
        float w = static_cast<float>(texture.getSize().x);
        float h = static_cast<float>(texture.getSize().y);

        quad.setPrimitiveType(sf::Quads);
        quad.resize(4);

        // Позиции вершин в форме ромба
        quad[0].position = { 0.f, h / 1.f };    // Лево
        quad[1].position = { w / 1.5f, 0.f };    // Верх
        quad[2].position = { w, h / 2.f };      // Право
        quad[3].position = { w / 2.f, h };      // Низ

        // Текстурные координаты (берём весь квадрат)
        quad[0].texCoords = { 0.f, 0.f };
        quad[1].texCoords = { w, 0.f };
        quad[2].texCoords = { w, h };
        quad[3].texCoords = { 0.f, h };
    }

    void setPosition(float x, float y) {
        sf::Vector2f offset(x, y);
        float w = static_cast<float>(texture.getSize().x);
        float h = static_cast<float>(texture.getSize().y);

        
        quad[0].position = offset + sf::Vector2f(0.f, h / 2.f);
        quad[1].position = offset + sf::Vector2f(w / 2.f, 0.f);
        quad[2].position = offset + sf::Vector2f(w, h / 2.f);
        quad[3].position = offset + sf::Vector2f(w / 2.f, h);
    }



    void draw(sf::RenderWindow& win) {
        sf::RenderStates states;
        states.texture = &texture;
        win.draw(quad, states);
    }

private:
    sf::Texture texture;
    sf::VertexArray quad;
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
    float angle = 0; 
    float fov = 360;  
    float speed = 5.0f;
    float x, y;
    bool isDraw = false;

    void TestLight(sf::RenderWindow& win, const std::vector<Wall>& walls) { // сделать нормальное отсечение стен
        int numRays = 720; // тестово въебал 720 потом накинуть больше +- 3000, увеличить длину лучей и дальность освещения 
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
        
    }

    

    void drawDelay() {

     }
};
