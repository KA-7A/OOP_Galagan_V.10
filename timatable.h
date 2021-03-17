//
// Created by Администратор on 15.03.2021.
//
/*
 * График движения поезда: показывает, какие станции этот поезд проходит в какое время. Программа должна уметь
 * оценивать корректность графика, сформировав человекочитаемый список найденных в нём ошибок.
 */

#ifndef OOP_2_TIMATABLE_H
#define OOP_2_TIMATABLE_H


#include <map>
#include <vector>
#include "line.h"

// Include JSON parser
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <vector>


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
    void getTimetableFromFile(const char *filename) {
        m_timetable.clear();
        FILE *source = fopen(filename, "r");
        assert(source);
        char readBuffer[65536];
        FileReadStream is(source, readBuffer, sizeof (readBuffer));
        Document timetableObj;
        timetableObj.ParseStream(is);
        fclose(source);

        assert(timetableObj.IsObject());
        assert(timetableObj.HasMember("timetable") && timetableObj["timetable"].Size() >0);
        //const Value& stop = timetableObj["timetable"][0];
        for (unsigned int i = 0; i < timetableObj["timetable"].Size(); i++)
        {
            assert(timetableObj["timetable"][i].IsObject());
            const Value& stop = timetableObj["timetable"][i];
            assert(stop.HasMember("name") && stop["name"].IsString());
            assert(stop.HasMember("time") && (stop["time"].IsDouble() || stop["time"].IsInt()));
            assert(stop["time"].GetDouble() >= 0);
            m_timetable.emplace_back(stop["time"].GetInt(), stop["name"].GetString());

        }
    };
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


#endif //OOP_2_TIMATABLE_H
