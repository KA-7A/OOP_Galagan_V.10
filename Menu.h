//
// Created by ka_7a on 02.03.2021.
//


#ifndef OOP_2_MENU_H
#define OOP_2_MENU_H
#include "line.h"
#include "timetable.h"
#include <iostream>
#include <limits>
#include <cstdio>
#include <utility>

class Menu {
private:
    std::vector<Line*> Lines;
public:
    Menu (std::vector<Line*> linesList) // Это надо запихнуть в отдельную функцию и спрятать в отдельный файл. Не понимаю, как.
    {
        callMenu(std::move(linesList));
    }
    void callMenu(std::vector<Line*> linesList);
    int drawStations(int mode) const ;
    static void drawMainMenu();
    static void printResult(int res);
    void test_Timetable(std::vector<Line*> linesList);
};


#endif //OOP_2_MENU_H
