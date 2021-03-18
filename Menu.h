//
// Created by ka_7a on 02.03.2021.
//


#ifndef OOP_2_MENU_H
#define OOP_2_MENU_H
#include "line.h"
#include "tests.h"
#include "timetable.h"
#include <iostream>
#include <limits>
#include <cstdio>

class Menu {
private:
    std::vector<Line*> Lines;
public:
    Menu (std::vector<Line*> linesList) // Это надо запихнуть в отдельную функцию и спрятать в отдельный файл. Не понимаю, как.
    {
        test_Timetable(linesList);
        return;

        Lines = linesList;
        int choice = 1;
        while (choice) {
            drawMainMenu();
            // Следующие две строчки спасают программу от кривого ввода
           scanf("%d", &choice);

            switch (choice) {
                case 1: {
                    drawStations(0);  // mode 0 for full  info
                                            // mode 1 for short info
                    break;
                }
                case 2: {
                    std::cout << "This part is still under development." << std::endl;
                    break;
                }
                case 3: {
                    int lineNum = drawStations(1);
                    if (lineNum == -1) break;
                    std::cout << " Which stations are you interested in? Type it's numbers \n >>";
                    int n1, n2;
                    scanf("%d %d", &n1, &n2);
                    if ((n1 / 1000) != (lineNum + 1) || (n2 / 1000) != (lineNum + 1))
                        std::cout << " Incorrect input: These stations do not belong to the selected line "
                                  << std::endl;
                    else {
                        double minTime = Lines[lineNum]->calculateTravelTime_min(n1, n2);
                        double maxTime = Lines[lineNum]->calculateTravelTime_max(n1, n2);
                        if (!(minTime == -1 || maxTime == -1))
                            std::cout << "min time: " << minTime << " || max time: " << maxTime << std::endl;
                    }
                    break;
                }
                case 4:
                {
                    Timetable t;
                    t.getTimetableFromFile("./Saves/timetable_1.json");
                    t.printTimetable();
                    printResult(t.checkTimetable(linesList));
                    break;
                }
                case 98: {
                    test_Timetable(linesList);
                    break;
                }
                case 99:
                {
                    //test_Station_class t();
                    break;
                }
                case 0: {   // Кнопка в меню на случай, если мы захотим выйти из программы
                    std::cout << " **********************************" << std::endl;
                    std::cout << " *    Bye, see you next time      *" << std::endl;
                    std::cout << " **********************************" << std::endl;
                    return ;
                }
                default: {
                    std::cin.clear();
                    continue;
                }
            }
        }
    }
    int drawStations(int mode) const ;
    static void drawMainMenu() {
        std::cout << " ************** Menu **************" << std::endl;
        std::cout << " * 1.  Show all stations          *" << std::endl;
        std::cout << " * 2.  Add the station to line    *" << std::endl;
        std::cout << " * 3.  Calculate travel time      *" << std::endl;
        std::cout << " * 4.  Timetable tests            *" << std::endl;
        std::cout << " *                                *" << std::endl;
        std::cout << " * 98. Do timetable tests!        *" << std::endl;
        std::cout << " * 99. Do tests!                  *" << std::endl;
        std::cout << " *                                *" << std::endl;
        std::cout << " * 0. Exit                        *" << std::endl;
        std::cout << " **********************************" << std::endl << " >> ";
    }
    static void printResult(int res) {
        std::cout << "Test timetable test's result:";
        switch (res) {
            case 0: {
                std::cout << " Ok! " << std::endl << std::endl;
                break;
            }
            case -1: {
                std::cout << " Error: Station(s) with such name(s) was(were) not found in line " << std::endl << std::endl;
                break;
            }
            case -2: {
                std::cout << " Error: Train reached the end of line but not the route!" << std::endl << std::endl;
                break;
            }
            case -3: {
                std::cout << " Error: Wrong time in timetable!" << std::endl << std::endl;
                break;
            }
            case -4: {
                std::cout << " Error: Timetable is empty!" << std::endl << std::endl;
                break;
            }
            case -5:
            {
                std::cout << " Error: Timetable has only one strip! " << std::endl << std::endl;
                break;
            }
            case -6:
            {
                std::cout << " Error: Train missed the station/ changed the direction/ try to reach the unknown station \n\n";
                break;
            }
            default: {
                std::cout << " Strange things... How did u got that?" << std::endl << std::endl;
                break;
            }
        }
    }

    void test_Timetable(std::vector<Line*> linesList)
    {
        for (int i = 1; i <= 8; i++)
        {
            Timetable t;
            std::string filename = "./Saves/timetable_" + std::to_string(i) + ".json";
            t.getTimetableFromFile(filename.c_str());
            t.printTimetable();
            printResult(t.checkTimetable(linesList));
        }
    }
};


#endif //OOP_2_MENU_H
