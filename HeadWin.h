#pragma once
#include <Windows.h>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Walls.h"
#include "ControlEng.h"
#include <commdlg.h> 
#include <locale>   
#include "Object.h"
#include "HeadObj.h"

std::vector<Wall> walls;
std::vector <Player> player; 
std::vector <Object> objects;

void saveToFile(const std::string& filename) {
    std::ofstream out(filename);
    for (const auto& w : walls) {
        out << w.wallX.x << " " << w.wallX.y << " "
            << w.wallY.x << " " << w.wallY.y << " "
            << static_cast<int>(w.coloorWallq.r) << " "
            << static_cast<int>(w.coloorWallq.g) << " "
            << static_cast<int>(w.coloorWallq.b) << "\n";
    }
}

void loadFromFile(const std::string& filename) {
    walls.clear();
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }
    float x1, y1, x2, y2;
    int r, g, b;
    while (in >> x1 >> y1 >> x2 >> y2 >> r >> g >> b) {
        Wall wall(sf::Vector2f(x1, y1), sf::Vector2f(x2, y2), 1); // colorIndex можно установить по умолчанию
        wall.coloorWallq = sf::Color(r, g, b);
        walls.push_back(wall);
    }
    if (in.fail() && !in.eof()) {
        std::cerr << "Error: Failed to read data from file " << filename << std::endl;
    }
}

std::string ShowSaveDialog(HWND hwnd) {
    OPENFILENAMEA ofn;
    char szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrDefExt = "txt";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    if (GetSaveFileNameA(&ofn) == TRUE) {
        return ofn.lpstrFile; // Возвращает выбранный путь к файлу
    }
    return ""; // Если пользователь отменил сохранение
}

std::string ShowOpenDialog(HWND hwnd, int fileType = 0) {
    OPENFILENAMEA ofn;
    char szFile[260] = { 0 };
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);

    const char* filter = nullptr;
    switch (fileType) {
    case 1:
        filter = "PNG Files (*.png)\0*.png\0All Files (*.*)\0*.*\0";
        break;
    case 2:
        filter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
        break;
    case 3:
        filter = "JPEG Files (*.jpeg)\0*.jpeg\0All Files (*.*)\0*.*\0";
        break;
    default:
        filter = "All Files (*.*)\0*.*\0";
        break;
    }

    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn) == TRUE) {
        return ofn.lpstrFile;
    }
    return "";
}


void win() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), L"Engine", sf::Style::Default);

    sf::Image icon;
    icon.loadFromFile("Images/Xui/TestIcon.png");
    window.setIcon(358, 1080, icon.getPixelsPtr());
    
    sf::CircleShape player(30);
    sf::RectangleShape enemy(sf::Vector2f(50, 50));
    player.setFillColor(sf::Color::Green);
    player.setPosition(100, 100);
    enemy.setFillColor(sf::Color::Red);
    enemy.setPosition(300, 100);

    window.setVerticalSyncEnabled(true);

    MouseWall mouse;
    int currentMode = 0;  // 0 - стены, 1 - игрок, 2 - враг
    
    Player cam(100,100);
    Enemy en(50, 50);

    HWND hwnd = window.getSystemHandle();
    AddMenu(hwnd);

    while (window.isOpen()) {
        MSG msg;
        while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_COMMAND) {
                switch (LOWORD(msg.wParam)) {
                case ID_FILE_NEW: {
                        if (!walls.empty() || player.getPosition().x >= 0 || enemy.getPosition().x >= 0) {
                        int result = MessageBox(hwnd, L"Создать новый файл? Несохраненные данные будут потеряны.",
                            L"Подтверждение", MB_YESNO | MB_ICONQUESTION);
                        if (result == IDNO) break;
                    }

                    walls.clear();
                    std::cout << "New file created" << std::endl;
                    break;
                }
                case ID_FILE_SAVE_US: {
                    std::string filePath = ShowSaveDialog(hwnd);
                    if (!filePath.empty()) {
                        saveToFile(filePath);
                        std::cout << "Saved to: " << filePath << std::endl;
                    }
                    break;
                }
                case ID_FILE_SAVE: {
                    int counter = 1;
                    std::string savePath;
                    std::ifstream testFile;
                    bool exists;
                    do {
                        savePath = "map_" + std::to_string(counter) + ".txt";
                        counter++;
                        testFile.open(savePath);
                        exists = testFile.is_open();
                        testFile.close();
                    } while (exists);

                    saveToFile(savePath);
                    std::cout << "Saved to " << savePath << std::endl;
                    break;
                }
                case ID_FILE_LOAD: {
                    std::string filePath = ShowOpenDialog(hwnd);
                    if (!filePath.empty()) {
                        loadFromFile(filePath);
                        std::cout << "Loaded from: " << filePath << std::endl;
                    }
                    break;
                }
                case ID_FILE_EXIT:
                    window.close();
                    break;
                case ID_FILE_PLAYER: {
                    int result = MessageBox(hwnd, L"Максимум может быть только один игрок, обязательно проверь.",
                        L"Подтверждение", MB_YESNO | MB_ICONQUESTION);
                    if (result == IDNO) break;
                    std::string filePath = ShowOpenDialog(hwnd, 1);  // 1 - PNG
                    std::cout << "Player selected and succes load" << std::endl;
                    if (!filePath.empty()) {
                        if (cam.playerTexture.loadFromFile(filePath)) {
                            cam.playerSprite.setTexture(cam.playerTexture);
                            std::cout << "Player texture loaded" << std::endl;
                        }
                        else {
                            std::cerr << "Failed to load texture!" << std::endl;
                        }
                    }
                    break;
                }
                case ID_FILE_WALL:
                    std::cout << "Wall selected!" << std::endl;
                    currentMode = 0;  
                    break;
                case ID_FILE_ENEMY: {
                    currentMode = 2;
                    std::string filePath = ShowOpenDialog(hwnd, 1);
                    std::cout << "Enemy selected and succes load" << std::endl;
                    if (!filePath.empty()) {
                        if (en.enemyTexture.loadFromFile(filePath)) {
                            en.enemySprite.setTexture(en.enemyTexture);
                            std::cout << "Enemy texture loaded" << std::endl;
                        }
                        else {
                            std::cerr << "Failed to load texture!" << std::endl;
                        }
                    }
                }
                    break;
                case ID_FILE_CLOSET:
                    std::cout << "Closet selected" << std::endl;
                    break;
            
                case ID_FILE_TABLE: 
                    std::cout << "Closet table" << std::endl;
                    break;
                
                case ID_FILE_COMMODE: 
                    std::cout << "Closet commode" << std::endl;
                    break;
                
                case ID_FILE_CHAIR: 
                    std::cout << "Closet chair" << std::endl;
                    break;
                
                case ID_FILE_DOOR: 
                    std::cout << "Closet door" << std::endl;
                    break;
                
                }
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();

            if (currentMode == 0) {
                mouse.handleEvent(window, event);
            }
            /*else if (currentMode == 1) {
                  window.draw(player);
            }*/
            else if (currentMode == 2) {

            }
        }

        // Отрисовка
        window.clear();
        
        if (currentMode == 0) {
            mouse.drawPreview(window);
            
        }
        else if (currentMode == 1) {
       
        }
        else if (currentMode == 2) {
            
        }


      
        /*window.draw(player);
        window.draw(enemy);*/
        for (const auto& wall : walls) {
            wall.draw(window);  // Рисуем все сохраненные стены
        }
        cam.update();
        cam.castRays(window, walls);
        window.draw(en.enemySprite);
        window.draw(cam.playerSprite);

        window.display();
    }
}
