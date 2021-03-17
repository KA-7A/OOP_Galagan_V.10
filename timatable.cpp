//
// Created by Администратор on 15.03.2021.
//

#include "timatable.h"

int Timetable::checkTimetable(std::vector<Line *> lines) {
    if (m_timetable.empty()) return -4;
    auto it = m_timetable.begin();
    Station *res = nullptr;
    int i;
    for (i = 0; i < lines.size(); i++) { // Ищем станцию в каждой ветке
        res = lines[i]->findStationByName(it->second);
        if (res)    // Если нашли
            break;  // Выходим из цикла
    }
    if (!res) return -1; // Проверяем, что станция нашлась. Если станции с таким именем нет ни в одно из веток, выходим
    /*
     * Че вообще внизу происходит:
     * Мы нашли стартовую станцию в нашем маршруте (res)
     * Дальше пытаемся определить направление движения поезда
     * Если он поехал вправо, то должен доехать до самого конца своего маршрута, ничего не пропуская и вообще
     */
    it++;
    while (res != nullptr && res->getRightAddr() && res->getRightName() == it->second && it != m_timetable.end()) {
        auto tmp = it;
        it++;
        if (res->getRightSpan().getTime_min() > abs(tmp->first - it->first) || res->getRightSpan().getTime_max() < abs(tmp->first - it->first) )
            return -3;
        res = res->getRightAddr();
    }
    // Это значит, что поезд доехал до конца маршрута И не вышел за пределы ветки
    if (it == m_timetable.end() && res) return 0;
        // Это значит, что поезд сдвинулся в вправо, но не доехал до конца маршрута/ он попал за границы ветки
    else if (it != m_timetable.begin()) return -2;
    // Если всё же поезд не сдвинулся, значит ему в другую сторону
    while (res != nullptr && res->getLeftAddr() && res->getLeftName() == it->second && it != m_timetable.end()) {
        auto tmp = it;
        it++;
        if (res->getRightSpan().getTime_min() > abs(tmp->first - it->first) || res->getRightSpan().getTime_max() < abs(tmp->first - it->first) )
            return -3;

        res = res->getLeftAddr();
    }
    if (it == m_timetable.end() && res) return 0;    // Аналогично, если он доехал до конца без происшествий, то ОК
    else return -1;  // Если нет, то в расписании бред и поезд пытается ехать по какой-то неправильной ветке.

}
/*
void Timetable::getTimetableFromFile(const char *filename) {
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
}*/

//void Timetable::printTimetable() const
