//  Тут будет имплементация функций, в которых просиходит тестирование классов
// Created by ka_7a on 09.03.2021.
//

#ifndef RAILWAY_TESTS
#define RAILWAY_TESTS
#include "station.h"
#include <cassert>
#include <iostream>

class test_Station_class
{
    /*
     * Эта функция тестирует базовый класс Station в отрыве
     * от соседних станций. Часть методов работает только если
     * соседние станции есть, и эта станция является частью
     * связанного списка.
     */
    test_Station_class() {
        std::cout << " ##### Station class test ##### " << std::endl;
        std::cout << " Default station :";
        Station defaultStation;
        assert(defaultStation.getName() == "Unnamed");
        assert(defaultStation.getNumber() == 0);
        assert(defaultStation.getTraffic() == 0);
        assert(defaultStation.isCrossing() == false);
        assert(defaultStation.getRightAddr() == nullptr && defaultStation.getLeftAddr() == nullptr);
        std::cout << " OK! " << std::endl;

        std::cout << " Named station   :";
        Station namedStation(10, 20, "St #10");
        assert(namedStation.getName() == "St #10");
        assert(namedStation.getNumber() == 10);
        assert(namedStation.getTraffic() == 20);
        assert(namedStation.isCrossing() == false);
        assert(namedStation.getRightAddr() == nullptr && namedStation.getLeftAddr() == nullptr);
        std::cout << " OK! " << std::endl;

        std::cout << " Named station edit test: ";
        namedStation.setNumber(100);
        assert(namedStation.getNumber() == 100);
        namedStation.setName("New name");
        assert(namedStation.getName() == "New name");
        namedStation.setTraffic(500);
        assert(namedStation.getTraffic() == 500);
        std::cout << " OK! " << std::endl;

        assert(!(namedStation == defaultStation));
        assert(defaultStation < namedStation);
    }
};

#endif