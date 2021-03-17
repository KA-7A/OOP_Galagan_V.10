//
// Created by Администратор on 15.03.2021.
//

#include "timetable.h"

int Timetable::checkTimetable(std::vector<Line *> lines) {
    if (m_timetable.empty())        return -4;  // Если расписание совсем пустое
    if (m_timetable.size() == 1)    return -5;  // Если расписание состоит всего из одного пункта
    int it = 0;
    Station *res = nullptr;
    for (int i = 0; i < lines.size(); i++) { // Ищем станцию в каждой ветке
        res = lines[i]->findStationByName(m_timetable[0].second);
        if (res)    // Если нашли
            break;  // Выходим из цикла
    }
    if (!res) return -1; // Проверяем, что станция нашлась. Если станции с таким именем нет ни в одно из веток, выходим
    /*
     * Че вообще внизу происходит:
     * Мы нашли стартовую станцию в нашем маршруте (res)
     * Дальше пытаемся определить направление движения поезда
     * Если он поехал вправо, то должен доехать до самого конца своего маршрута, ничего не пропуская
     */
    //it++;   //              |Проверяем, что поехал вправо Проверяем, что поехал вправо|

    if (res->getRightAddr() && res->getRightName() == m_timetable[1].second){
        while (res != nullptr && res->getRightAddr() && it+1 < m_timetable.size() && res->getRightName() == m_timetable[it+1].second && it < m_timetable.size() ) {
            // Ошибка, если поезд слишком много/мало времени проводит на перегоне
            if (res->getRightSpan().getTime_min() > abs(m_timetable[it].first - m_timetable[it+1].first) ||
                res->getRightSpan().getTime_max() < abs(m_timetable[it].first - m_timetable[it+1].first) )
                return -3;
            res = res->getRightAddr();
            it++;
        }
        // Это значит, что поезд доехал до конца маршрута И не вышел за пределы ветки
        if (!res) return -2;                            // Вышли за границу ветки и не дошли до конца маршрута
        // 36 строка не уверен, что оно реально так должно быть.
        if ((it + 1) == m_timetable.size()) return 0;
            // Это значит, что поезд сдвинулся в вправо, но не доехал до конца маршрута/ он попал за границы ветки
        else if (it !=0) return -2;
    }
    else if (res->getLeftAddr() && res->getLeftName() == m_timetable[1].second) { // Иначе если поезд сразу поехал налево
        while (res != nullptr && res->getLeftAddr() && it+1 < m_timetable.size() &&res->getLeftName() == m_timetable[it+1].second) {
            // Ошибка, если поезд слишком много/мало времени проводит на перегоне
            if (res->getRightSpan().getTime_min() > abs(m_timetable[it].first - m_timetable[it + 1].first) ||
                res->getRightSpan().getTime_max() < abs(m_timetable[it].first - m_timetable[it + 1].first))
                return -3;
            res = res->getLeftAddr();
            it++;
        }
        if (it == m_timetable.size() && res) return 0;    // Аналогично, если он доехал до конца без происшествий, то ОК
        else return -2;  // Если нет, то в расписании бред и поезд пытается ехать по какой-то неправильной ветке.
    }
    else return -6;
}


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
    assert(timetableObj.HasMember("timetable"));
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
}

//void Timetable::printTimetable() const
