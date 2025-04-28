#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include "HeadWin.h"
#include "Walls.h"

//class Player {
//    public: 
//        float x, y, fov;
//        sf::CircleShape circle;
//
//        Player(float x, float y,float fov, float h) {
//           
//            sf::CircleShape circle(h);
//            circle.setPosition(x, y);
//        }
//
//        void draw(sf::RenderWindow& window) {
//            window.draw(circle);
//
//        }
//
//        void move() {
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
//                x++;
//            }
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
//                x--;
//            }
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
//                y++;
//            }
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
//                y--;
//            }
//        }
//
//        void cam() {
//            
//        }
//};


int main(){
    win();
    return 0;


    //    /* if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    //         if (lu) { a1.y += 1; a4.y -= 1; a2.y -= 1; a3.y += 1; if (a1.y > 400) lu = false; }
    //         else { a1.y -= 1; a4.y += 1; a2.y += 1; a3.y -= 1; if (a1.y < 50) lu = true; }
    //         std::cout << a1.y << std::endl;
    //     }*/
        //     /* convex.setPoint(0, a1);
    //      convex.setPoint(1, a2);
    //      convex.setPoint(2, a3);
    //      convex.setPoint(3, a4);*/

    //      // window.draw(circle);
    //       //window.draw(myLines);
    //       //p.Ray(window);
    //       /*p.Interaction(window);
    //       p.Draw(window);*/


    //       //window.draw(sprite);
    //       //window.draw(convex);
    //      // p.draw(window);

    //    window.display();

    //    // p.move();
    //}
    //return 0;
    //*/
}

//int game() {
//    sf::RenderWindow window(sf::VideoMode(2048, 1920), L"ААА", sf::Style::Default);
//    //Player p("Character.jpeg",300,300,20);
//
//    sf::CircleShape circle(100.f);
//
//    window.setVerticalSyncEnabled(true);
//
//    /*sf::Vector2f a1{ 100,100 };
//    sf::Vector2f a2{ 500,200 };
//    sf::Vector2f a3{ 500,200 };
//    sf::Vector2f a4{ 100,300 };
//    bool lu = true;
//    sf::ConvexShape convex;
//    convex.setPointCount(4);
//    convex.setFillColor(sf::Color::Magenta);
//    */
//
//    int x = 0, y = 0;
//
//    sf::Mouse::getPosition();
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
//                window.close();
//        }
//
//        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
//            window.clear();
//            window.clear(sf::Color::Blue);
//        }
//        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
//            sf::Mouse::getPosition();
//
//        }
//
//        /* if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
//             if (lu) { a1.y += 1; a4.y -= 1; a2.y -= 1; a3.y += 1; if (a1.y > 400) lu = false; }
//             else { a1.y -= 1; a4.y += 1; a2.y += 1; a3.y -= 1; if (a1.y < 50) lu = true; }
//             std::cout << a1.y << std::endl;
//         }*/
//
//
//         /* convex.setPoint(0, a1);
//          convex.setPoint(1, a2);
//          convex.setPoint(2, a3);
//          convex.setPoint(3, a4);*/
//
//          // window.draw(circle);
//           //window.draw(myLines);
//           //p.Ray(window);
//           /*p.Interaction(window);
//           p.Draw(window);*/
//
//
//           //window.draw(sprite);
//           //window.draw(convex);
//          // p.draw(window);
//
//        window.display();
//
//        // p.move();
//    }
//    return 0;
//}