//  Тут будет имплементация функций, в которых просиходит тестирование классов
// Created by ka_7a on 09.03.2021.
//

#include "station.h"
#include <cassert>
#include <iostream>

void test_Station_class()
{
    /*
     * Эта функция тестирует базовый класс Station в отрыве
     * от соседних станций. Часть методов работает только если
     * соседние станции есть, и эта станция является частью
     * связанного списка.
     */
    std::cout<< " ##### Station class test ##### " << std::endl;
    std::cout<< " Default station :";
    Station defaultStation;
    assert(defaultStation.getName() == "Unnamed");
    assert(defaultStation.getNumber() == 0);
    assert(defaultStation.getTraffic() == 0);
    assert(defaultStation.isCrossing() == false);
    assert(defaultStation.getRightAddr() == nullptr && defaultStation.getLeftAddr() == nullptr);
    std::cout<< " OK! " << std::endl;

    std::cout<< " Named station   :";
    Station namedStation(10, 20, "St #10");
    assert(namedStation.getName() == "St #10");
    assert(namedStation.getNumber() == 10);
    assert(namedStation.getTraffic() == 20);
    assert(namedStation.isCrossing() == false);
    assert(namedStation.getRightAddr() == nullptr && namedStation.getLeftAddr() == nullptr);
    std::cout<< " OK! " << std::endl;

    std::cout << " Named station edit test: ";
    namedStation.changeToType(false);
    assert(namedStation.isCrossing() == true);
    namedStation.setNumber(100);
    assert(namedStation.getNumber() == 100);
    namedStation.setName("New name");
    assert(namedStation.getName() == "New name");
    namedStation.setTraffic(500);
    assert(namedStation.getTraffic() == 500);
    std::cout<< " OK! " << std::endl;

    assert(!(namedStation==defaultStation));
    assert(defaultStation<namedStation);
}

int main()
{
    test_Station_class();
    return 0;
}