//
// Created by Администратор on 15.03.2021.
//
/*
 * График движения поезда: показывает, какие станции этот поезд проходит в какое время. Программа должна уметь
 * оценивать корректность графика, сформировав человекочитаемый список найденных в нём ошибок.
 */

#ifndef OOP_2_TIMETABLE_H
#define OOP_2_TIMETABLE_H


#include <map>
#include <vector>
#include <vector>
#include "line.h"


class Timetable {
private:
    //       <Порядок в расписании,<время, имя станции>> -- такая вот структура расписания
    //std::vector<timeTableStrip> m_timetable;
    std::vector<std::pair<int, std::string>> m_timetable;
    /*
     * Устройство:
     *      Время -- станция
     *      map сам всё отсортирует по времени.. Поэтому и сделали
     */
public:
    Timetable(){};
    // Метод, заполняющий наш вектор из файла
    void getTimetableFromFile(const char *filename);
    void printTimetable() const {
        int ind = 0;
        printf("  № | time | Station \n");
        for (auto i = m_timetable.begin(); i!=m_timetable.end(); i++, ind++)
            printf("%3d | %3d  | %s\n", ind, i->first, (i->second).c_str());
            //std::cout << "№ " << ind << " time: " << i->first << " Station: " << i->second << std::endl;
    };
    // ## Метод, проверяющий расписание на общую адекватность
    /*
     * Ошибки времени бывают:
     * 1. Слишком маленькое/ большое время на перегоне между станциями
     * 2. Поезд находится в одно время в двух местах
     * Вообще говоря, будет достаточно только первого признака. Понятно почему
     *
     * Еще бывает ошибка порядка станций, когда поезд что-то пропускает/ пытается доехать до неправильной станции
     */
    int checkTimetable(std::vector<Line*> lines);


};


#endif //OOP_2_TIMETABLE_H
