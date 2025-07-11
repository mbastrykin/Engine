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
#define ID_LOAD_FILE_BACKGROUND 15
#define ID_LOAD_LIGHT 16
#define ID_LOAD_FLASHLIGHT 17
#define ID_LOAD_LAMP 18
#define ID_LOAD_FILE_TEXT 19
//#define ID_LOAD_TESTGAME 19


void AddMenu(HWND hwnd) {

    HMENU hMenuBar = CreateMenu();             // �����

    HMENU hMenuFile = CreatePopupMenu();
    AppendMenu(hMenuFile, MF_STRING, ID_FILE_NEW, L"����� ����");
    AppendMenu(hMenuFile, MF_STRING, ID_FILE_SAVE, L"���������");
    AppendMenu(hMenuFile, MF_STRING, ID_FILE_SAVE_US, L"��������� ���");
    AppendMenu(hMenuFile, MF_STRING, ID_FILE_LOAD, L"���������");
    AppendMenu(hMenuFile, MF_STRING, ID_FILE_EXIT, L"�����");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hMenuFile, L"����");


    HMENU hMenuObjects = CreatePopupMenu();
    AppendMenu(hMenuObjects, MF_STRING, ID_FILE_PLAYER, L"�����");
    AppendMenu(hMenuObjects, MF_STRING, ID_FILE_WALL, L"����� �����");
    AppendMenu(hMenuObjects, MF_STRING, ID_FILE_ENEMY, L"����");
    AppendMenu(hMenuObjects, MF_STRING, ID_FILE_DELETE, L"��������");
    AppendMenu(hMenuObjects, MF_STRING, ID_LOAD_FILE_BACKGROUND, L"���");
    AppendMenu(hMenuObjects, MF_STRING, ID_LOAD_FILE_TEXT, L"�����");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hMenuObjects, L"�������");

    HMENU hMenuInterior = CreatePopupMenu();
    AppendMenu(hMenuInterior, MF_STRING, ID_FILE_CLOSET, L"����");
    AppendMenu(hMenuInterior, MF_STRING, ID_FILE_TABLE, L"����");
    AppendMenu(hMenuInterior, MF_STRING, ID_FILE_COMMODE, L"�����");
    AppendMenu(hMenuInterior, MF_STRING, ID_FILE_CHAIR, L"����");
    AppendMenu(hMenuInterior, MF_STRING, ID_FILE_DOOR, L"�����");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hMenuInterior, L"��������");

    HMENU hMenulighting = CreatePopupMenu();
    AppendMenu(hMenulighting, MF_STRING, ID_LOAD_LIGHT, L"����");
    AppendMenu(hMenulighting, MF_STRING, ID_LOAD_LAMP, L"�����");
    AppendMenu(hMenulighting, MF_STRING, ID_LOAD_FLASHLIGHT, L"�������");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hMenulighting, L"���������");

   /* HMENU hMenuTestGame = CreatePopupMenu();
    AppendMenu(hMenuTestGame, MF_STRING, ID_LOAD_TESTGAME, L"������ �����");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hMenuTestGame, L"����");*/

    SetMenu(hwnd, hMenuBar);
}