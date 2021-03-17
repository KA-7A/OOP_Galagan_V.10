//
// Created by Администратор on 12.03.2021.
//

#include "Menu.h"
#include "cstdio"

int Menu::drawStations(int mode) const {
    int ans = -2;
    while (ans == -2) {
        std::cout << " ## Which line are you interested in? ##" << std::endl;
        for (int i = 0;
             i < Lines.size(); std::cout << " #  " << i << ". " << Lines[i]->getName() << ";" << std::endl, i++);
        std::cout << " #\n # -1. Return to main menu " << std::endl << " >> ";
        scanf("%d", &ans);
        if (ans == -1)
            return -1;
        if (ans >= 0 && ans < Lines.size()) {
            if (!mode)
                Lines[ans]->printFullAllStationsInfo_list();
            else
                Lines[ans]->printShortAllStationsInfo_list();
            return ans;
        } else {
            std::cout << "Incorrect choice!" << std::endl;
            ans = -2;
        }
    }
    return -1;
};

