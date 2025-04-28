#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Walls.h"
#include "Object.h"

extern std::vector<Wall> walls;

class MouseWall {
public:
    bool isDraw = false;
    int colorDraw = 1;
    sf::Vector2f start;

    bool isPointNearLine(sf::Vector2f point, sf::Vector2f lineStart, sf::Vector2f lineEnd, float radius) {
        float length = std::hypot(lineEnd.x - lineStart.x, lineEnd.y - lineStart.y);
        if (length < 0.0001f) return false;

        float u = ((point.x - lineStart.x) * (lineEnd.x - lineStart.x) +
            (point.y - lineStart.y) * (lineEnd.y - lineStart.y)) / (length * length);

        u = std::max(0.0f, std::min(1.0f, u));

        sf::Vector2f closest(
            lineStart.x + u * (lineEnd.x - lineStart.x),
            lineStart.y + u * (lineEnd.y - lineStart.y)
        );

        return std::hypot(point.x - closest.x, point.y - closest.y) <= radius;
    }

    void handleEvent(sf::RenderWindow& window, sf::Event event) {
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Right) {

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            for (auto it = walls.begin(); it != walls.end(); ) {
                if (isPointNearLine(mousePos, it->wallX, it->wallY, 10.0f)) { 
                    it = walls.erase(it);
                    std::cout << "Wall deleted" << std::endl;
                }
                else {
                    ++it;
                }
            }
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            start = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            isDraw = true;
        }

        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            if (isDraw) {
                sf::Vector2f end = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                walls.emplace_back(start, end, colorDraw);
                std::cout << "Wall created from (" << start.x << ", " << start.y << ") to ("
                    << end.x << ", " << end.y << ")\n";
                isDraw = false;
            }
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9) {
                colorDraw = event.key.code - sf::Keyboard::Num0;
            }
        }
    }

    void drawPreview(sf::RenderWindow& window) {
        if (isDraw) {
            sf::Vector2f current = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::Vertex line[] = {
                sf::Vertex(start, Wall().getWallColor(colorDraw)),
                sf::Vertex(current, Wall().getWallColor(colorDraw))
            };
            window.draw(line, 2, sf::Lines);
        }
    }
};

class ControlEng {
 
};