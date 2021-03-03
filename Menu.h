//
// Created by ka_7a on 02.03.2021.
//

#ifndef OOP_2_MENU_H
#define OOP_2_MENU_H
#include "line.h"

class Menu {
private:
    std::vector<Line> Lines;
public:
    Menu (std::vector<Line> linesList)
    {
        Lines = linesList;
        int choice = 1;
        while (choice)
        {
            drawMainMenu();
            std::cin >> choice;
            switch (choice){
                case 1:
                {
                    drawStations(0);
                    break;
                }
                case 3:
                {
                    int lineNum = drawStations(1);
                    std::cout << " Which stations are you interested in? Type it's numbers \n >>" ;
                    int n1, n2;
                    std::cin >> n1 >> n2;
                    if ( (n1 / 1000) != (lineNum + 1) || (n2 / 1000) != (lineNum + 1) )
                        std::cout << " Incorrect input: These stations do not belong to the selected line " << std::endl;
                    else
                    {
                        double minTime = Lines[lineNum].calculateTravelTime_min(n1, n2);
                        double maxTime = Lines[lineNum].calculateTravelTime_max(n1, n2);
                        if (!(minTime == -1 || maxTime == -1))
                            std::cout << "min time: " << minTime << " || max time: " << maxTime << std::endl;
                    }
                    break;
                }
                case 0:
                {
                    std::cout << "Bye, see you next time" << std::endl;
                    exit(0);
                }
                default:
                    continue;
            }
        }
    }
    int drawStations(int mode) const {
        int ans = -2;
        while (ans == -2) {
            std::cout << " ## Which line are you interested in? ##" << std::endl;
            for (int i = 0;
                 i < Lines.size(); std::cout << " #  " << i << ". " << Lines[i].getName() << ";" << std::endl, i++);
            std::cout << " #\n # -1. Return to main menu " << std::endl << " >> ";
            std::cin >> ans;
            if (ans == -1)
                return -1;
            if (ans >= 0 && ans < Lines.size()) {
                if (!mode)
                    Lines[ans].printFullAllStationsInfo_list();
                else
                    Lines[ans].printShortAllStationsInfo_list();
                return ans;
            }
            else {
                std::cout << "Incorrect choice!" << std::endl;
                ans = -2;
            }
        }
        return -1;
    }
    static void drawMainMenu() {
        std::cout << " ************** Menu **************" << std::endl;
        std::cout << " * 1. Show all stations           *" << std::endl;
        std::cout << " * 2. Add the station to line     *" << std::endl;
        std::cout << " * 3. Calculate travel time       *" << std::endl;
        std::cout << " *                                *" << std::endl;
        std::cout << " * 0. Exit                        *" << std::endl;
        std::cout << " **********************************" << std::endl << " >> ";}
};


#endif //OOP_2_MENU_H
