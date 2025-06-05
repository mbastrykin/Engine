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
#include <optional>

std::vector<Wall> walls;
std::vector <Player> player;
std::vector <Object> objects;
std::vector <Background> back;


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
        Wall wall(sf::Vector2f(x1, y1), sf::Vector2f(x2, y2), 1);
        wall.coloorWallq = sf::Color(r, g, b);
        walls.push_back(wall);
    }
    if (in.fail() && !in.eof()) {
        std::cerr << "Error: Failed to read data from file " << filename << std::endl;
    }
}

std::string LoadFileBackgroundFromFold(HWND hwnd, int fileType = 0) {
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

   /* sf::Image icon;
    icon.loadFromFile("Images/Xui/TestIcon.png");
    window.setIcon(358, 1080, icon.getPixelsPtr());*/

    window.setVerticalSyncEnabled(true);

    HWND hwnd = window.getSystemHandle();
    
    MouseWall mouse;

    Background* background = nullptr;
    Light* light = nullptr;

    Player* cam = nullptr;
    Enemy* en = nullptr;
    Text* text = nullptr;

    AddMenu(hwnd);

    int currentMode = 0;  // 0 - стены, 1 - игрок, 2 - враг

    while (window.isOpen()) {
        MSG msg;
        while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_COMMAND) {
                switch (LOWORD(msg.wParam)) {
                case ID_FILE_NEW: {
                    int result = MessageBox(hwnd, L"Создать новый файл? Несохраненные данные будут потеряны.",
                        L"Подтверждение", MB_YESNO | MB_ICONQUESTION);
                    if (result == IDNO) break;
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
                        cam = new Player(0, 0);
                        
                        if (cam->playerTexture.loadFromFile(filePath)) {
                            cam->playerSprite.setTexture(cam->playerTexture);
                            cam->setTextureSize(window);
                            std::cout << "Player texture loaded" << std::endl;
                            cam->isCreatePlayer = true;
                            
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
                    currentMode = 1;
                    std::string filePath = ShowOpenDialog(hwnd, 1);
                    std::cout << "Enemy selected and succes load" << std::endl;
                    if (!filePath.empty()) {
                        en = new Enemy(500, 500);
                        
                        if (en->enemyTexture.loadFromFile(filePath)) {
                            en->enemySprite.setTexture(en->enemyTexture);
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
                    std::cout << "Table selected" << std::endl;
                    break;

                case ID_FILE_COMMODE:
                    std::cout << "Commode selected" << std::endl;
                    break;

                case ID_FILE_CHAIR:
                    std::cout << "Chair selected" << std::endl;
                    break;

                case ID_FILE_DOOR:
                    std::cout << "Door selected" << std::endl;
                    break;

                case ID_LOAD_FILE_BACKGROUND: {
                    currentMode = 3;
                    std::string filePath = ShowOpenDialog(hwnd, 1);
                    std::cout << "Background selected and succes load" << std::endl;
                    if (!filePath.empty()) {
                        if (background) delete background;  // удаляем старый, если есть
                        background = new Background(0, 0);
                        background->setTextureAndScale(filePath, window);
                    }
                    break;
                }
                case ID_LOAD_LIGHT: {
                    currentMode = 4;
                    std::cout << "Light selected" << std::endl;
                    light = new Light(500, 500);

                }
               /* case ID_LOAD_FILE_TEXT: {
                    std::cout << "Text selected" << std::endl;
                    text = new Text(50,50);
                }*/
                /*case ID_LOAD_TESTGAME: {
                    TestGame();
                }*/
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


        if (background) {
            window.draw(background->backSprite);
        }
        
        if (cam) {
            cam->update();
            cam->castRays(window, walls);
            window.draw(cam->playerSprite);
        }
        if (en) {
            en->update();
            window.draw(en->enemySprite);
           
        }
        if (light) {
            light->TestLight(window,walls);
        }
        //for (const auto& wall : walls) {
        //    wall.draw(window);  // Рисуем все сохраненные стены
        //}
       
        
        for (const auto& wall : walls) {
            if (wall.colorWall == 1) {
                wall.draw(window);
            }
            if (wall.visibleWall) {
                sf::Vertex line[] = {
                    sf::Vertex(wall.wallX, wall.coloorWallq),
                    sf::Vertex(wall.wallY, wall.coloorWallq)
                };
                window.draw(line, 2, sf::Lines);
               
            }
        }
        window.display();
    }
    if (background) {
        delete background;
        background = nullptr;
    }
}