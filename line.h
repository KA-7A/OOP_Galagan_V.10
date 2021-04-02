/*
 * В этой версии я буду устраивать двусвязный список ну тупо потому что мне так проще. Пока что.
 * В будущем я планирую выполнить сложный вариант, тем более он вроде не такой уж и сложный.. Должен быть.
 */

/*
 * Линия: двусвязный список (лёгкий вариант) либо skip-list (сложный вариант), станции в котором упорядочены по
 * порядковому номеру. Необходимо уметь находить 2 соседние станции для любой указанной -- указанной как: по названию, номеру, ...?
 * (с временем следования  от одной и от другой), а также оценивать максимальное и минимальное время в пути между двумя станциями.
 */
// Created by ka_7a on 19.02.2021.
//

#ifndef RAILWAY_LINE_H
#define RAILWAY_LINE_H
#include <utility>
#include <vector>
#include <cassert>
#include "span.h"
#include "station.h"
#include "crossing_station.h"

// Add JSON parser:
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>

#include <cstdio>
#include <algorithm>

using namespace rapidjson;

class Line {
private:
    std::vector<Station*> m_Stations;   // Собственно, вектор, в котором будет храниться наш двусвязный список
    std::vector<Span>     m_Spans;      // Сделаем такую вот штуку, чтобы гарантированно сохранить все перегоны (чтобы ничто нигде не затерлось)
    Station *m_head, *m_tail;           // Указатели на начало и конец ветки.. (необходимый двусвязный список)
    std::string m_name;

    int checkLine() const {
        int counter = 0;
        for (int i = 0; i < m_Stations.size(); i++)
            if (m_Stations[i]->getRightAddr() == nullptr || m_Stations[i]->getLeftAddr() == nullptr)
                counter++;
        return counter;
    }

public:
    Line() {
        m_head = m_tail = nullptr;
    }

// ## Надо будет поменять названия, потому что они слегка не соответствуют тому, что должны делать ## //
    virtual inline void addStationToLine(Station* S){ m_Stations.push_back(S);      }

// ## Меняем кое-что внутри нашей ветки
    inline void writeName(std::string name) { m_name = std::move(name); }
    inline void spanPushBack(const Span& s) { m_Spans.push_back(s); }

// ## Получаем на выход информацию о станции
    std::string getName() const{ return m_name; }
    void printFullAllStationsInfo_list()  const{
        Station *head = m_head;
        while (head != nullptr)
        {
            head ->printFullInfo();
            head = head->getRightAddr();
        }
    }
    void printShortAllStationsInfo_list() const{
        Station *head = m_head;
        while (head != nullptr)
        {
            head ->printShortInfo();
            head = head->getRightAddr();
        }
    }
// ## Метод связывания вектора в связный список
    virtual void connectLine();

// ## Методы с расчетом максимального/ минималььного времени
    double calculateTravelTime_min (int n1, int n2) const;
    double calculateTravelTime_max (int n1, int n2) const;
    Station* findStationByName(const std::string &name)  // Функция, которая проверяет, есть ли станция с указанным именем в ветке
    {
        for (Station *cur = m_head; cur != nullptr; cur = cur ->getRightAddr())
            if (cur->getName() == name) return cur;
        return nullptr;
    }

    ~Line()
    {
        for (int i = m_Stations.size(); i >=0 ; i--)
            delete m_Stations[i];
    }

};

class S_Line: public Line {
private:
    // А тут как бы нечему храниться. Разница в том, что внутри m_Stations будет лежать S_Stations
public:

};


#endif //RAILWAY_LINE_H
