//
// Created by Администратор on 15.03.2021.
//

#include "timetable.h"

int Timetable::checkTimetable(std::vector<Line *> lines) {
    if (m_timetable.empty())        return -4;  // Если расписание совсем пустое
    if (m_timetable.size() == 1)    return -5;  // Если расписание состоит всего из одного пункта
    auto it = m_timetable.begin();
    Station *cur = nullptr;
    for (int i = 0; i < lines.size(); i++) { // Ищем станцию в каждой ветке
        cur = lines[i]->findStationByName(m_timetable[0].second);
        if (cur)    // Если нашли
            break;  // Выходим из цикла
    }
    if (!cur) return -1; // Проверяем, что станция нашлась. Если станции с таким именем нет ни в одно из веток, выходим
    /*
     * Че вообще внизу происходит:
     * Мы нашли стартовую станцию в нашем маршруте (res)
     * Дальше пытаемся определить направление движения поезда
     * Если он поехал вправо, то должен доехать до самого конца своего маршрута, ничего не пропуская
     *
     * cur -- указатель в векторе - линии
     * it  -- итератор  в векторе - расписании
     */
    //it++;
    // Если поезд мог поехать направо И поехал направо
    if (cur->getRightAddr() && cur->getRightName() == m_timetable[1].second){
        while (it != m_timetable.end())  // Пока не доберемся до конца маршрута
        {
            auto tmp = it;
            tmp++;
            if (tmp != m_timetable.end()) {  // Проверяем время переезда
                // Ошибка, если поезд слишком много/мало времени проводит на перегоне
                if (it->second != cur->getName() || tmp->second != cur->getRightName()) return -6;
                if (cur->getRightSpan().getTime_min() > abs(tmp->first - it->first) ||
                    cur->getRightSpan().getTime_max() < abs(tmp->first - it->first))
                    return -3;
            }
            if (!(cur->getRightAddr()) && tmp != m_timetable.end()) return -2;  // Доехали до конца ветки но не до конца маршрута
            cur = cur->getRightAddr();
            it++;
        }
        return 0;
    }
    else if (cur->getLeftAddr() && cur->getLeftName() == m_timetable[1].second){
        while (it != m_timetable.end())  // Пока не доберемся до конца маршрута
        {
            auto tmp = it;
            tmp++;
            if (tmp != m_timetable.end()) {  // Проверяем время переезда
                // Ошибка, если поезд слишком много/мало времени проводит на перегоне
                if (it->second != cur->getName() || tmp->second != cur->getLeftName()) return -6;
                if (cur->getLeftSpan().getTime_min() > abs(tmp->first - it->first) ||
                    cur->getLeftSpan().getTime_max() < abs(tmp->first - it->first))
                    return -3;
            }
            if (!(cur->getLeftAddr()) && tmp != m_timetable.end()) return -2;  // Доехали до конца ветки но не до конца маршрута
            cur = cur->getLeftAddr();
            it++;
        }
        return 0;
    }
    return -6;
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
