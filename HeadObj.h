#pragma once
#include <Windows.h>

#define ID_FILE_SAVE_US 0
#define ID_FILE_SAVE 7
#define ID_FILE_EXIT 1
#define ID_FILE_LOAD 2
#define ID_FILE_OBJECT 3
#define ID_FILE_PLAYER 4
#define ID_FILE_WALL 5
#define ID_FILE_ENEMY 6
#define ID_FILE_DELETE 8
#define ID_FILE_NEW 9
#define ID_FILE_CLOSET 10
#define ID_FILE_TABLE 11
#define ID_FILE_COMMODE 12
#define ID_FILE_CHAIR 13
#define ID_FILE_DOOR 14


void AddMenu(HWND hwnd) {

    HMENU hMenuBar = CreateMenu();             // Главное меню

    HMENU hMenuFile = CreatePopupMenu();
    AppendMenu(hMenuFile, MF_STRING, ID_FILE_NEW, L"Новый файл");
    AppendMenu(hMenuFile, MF_STRING, ID_FILE_SAVE, L"Сохранить");
    AppendMenu(hMenuFile, MF_STRING, ID_FILE_SAVE_US, L"Сохранить как");
    AppendMenu(hMenuFile, MF_STRING, ID_FILE_LOAD, L"Загрузить");
    AppendMenu(hMenuFile, MF_STRING, ID_FILE_EXIT, L"Выход");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hMenuFile, L"Файл");


    HMENU hMenuObjects = CreatePopupMenu();
    AppendMenu(hMenuObjects, MF_STRING, ID_FILE_PLAYER, L"Игрок");
    AppendMenu(hMenuObjects, MF_STRING, ID_FILE_WALL, L"Стены карты");
    AppendMenu(hMenuObjects, MF_STRING, ID_FILE_ENEMY, L"Враг");
    AppendMenu(hMenuObjects, MF_STRING, ID_FILE_DELETE, L"Удаление");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hMenuObjects, L"Объекты");

    HMENU hMenuInterior = CreatePopupMenu();
    AppendMenu(hMenuInterior, MF_STRING, ID_FILE_CLOSET, L"Шкаф");
    AppendMenu(hMenuInterior, MF_STRING, ID_FILE_TABLE, L"Стол");
    AppendMenu(hMenuInterior, MF_STRING, ID_FILE_COMMODE, L"Комод");
    AppendMenu(hMenuInterior, MF_STRING, ID_FILE_CHAIR, L"Стул");
    AppendMenu(hMenuInterior, MF_STRING, ID_FILE_DOOR, L"Дверь");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hMenuInterior, L"Интерьер");

    HMENU hMenulighting = CreatePopupMenu();
    AppendMenu(hMenulighting, MF_STRING, ID_FILE_NEW, L"Шкаф");
    AppendMenu(hMenulighting, MF_STRING, ID_FILE_SAVE, L"Стол");
    AppendMenu(hMenulighting, MF_STRING, ID_FILE_SAVE_US, L"Комод");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hMenulighting, L"Освещение");

    SetMenu(hwnd, hMenuBar);
}