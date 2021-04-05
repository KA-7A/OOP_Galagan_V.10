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
#include <list>
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
protected:
    std::vector<Span> m_spansList;  // Сделаем такую вот штуку, чтобы гарантированно сохранить все перегоны (чтобы ничто нигде не затерлось)
    std::vector<Station*>  m_line;  // Собственно, вектор, в котором будет храниться наш двусвязный список
    Station *m_head, *m_tail;       // Указатели на начало и конец ветки.. (необходимый двусвязный список)
private:
    std::string m_name;

    int checkLine() const {
        int counter = 0;
        for (unsigned int i = 0; i < m_line.size(); i++)
            if (m_line[i]->getRightAddr() == nullptr || m_line[i]->getLeftAddr() == nullptr)
                counter++;
        return counter;
    }

public:
    Line() {
        m_head = m_tail = nullptr;
    }

// ## Надо будет поменять названия, потому что они слегка не соответствуют тому, что должны делать ## //
     inline void addStationToLine(Station* S){
        // Надо сделать так, что при вставке нового элемента, список был всегда отсортирован
        auto pos = std::find_if(m_line.begin(), m_line.end(),
                               [&]( Station *it) { return (it->getNumber() > S->getNumber()); }) ;
        // Верхняя строчка вернет итератор на первую станцию, номер которой будет больше, чем она.
        // Тогда вставим элемент на эту позицию.
        m_line.emplace(pos, S);
    }

// ## Меняем кое-что внутри нашей ветки
    inline void writeName(std::string name) { m_name = std::move(name); }
    inline void spanPushBack(const Span& s) { m_spansList.push_back(s); }

// ## Получаем на выход информацию о станции
    std::string getName() const{ return m_name; }
    void printFullAllStationsInfo_list()  const{
        Station *head = m_head;
        while (head != m_tail)
        {
            head ->printFullInfo();
            head = head->getRightAddr();
        }
        head ->printFullInfo();
    }
    void printShortAllStationsInfo_list() const{
        Station *head = m_head;
        while (head != m_tail)
        {
            head ->printShortInfo();
            head = head->getRightAddr();
        }
        head ->printShortInfo();
    }
// ## Метод связывания вектора в связный список
    virtual void connectLine();

// ## Методы с расчетом максимального/ минималььного
// РАБОТАЮТ С УЖЕ СВЯЗНЫМ СПИСКОМ
    double calculateTravelTime_min (int n1, int n2) const;
    double calculateTravelTime_max (int n1, int n2) const;
    Station* findStationByName(const std::string &name)  // Функция, которая проверяет, есть ли станция с указанным именем в ветке
    {
        for (Station *cur = m_head; cur != nullptr; cur = cur ->getRightAddr())
            if (cur->getName() == name) return cur;
        return nullptr;
    }

    virtual ~Line()
    {
        for (int i = m_line.size(); i >=0 ; i--)
            delete m_line[i];
    }

};

class S_Line: public Line
{
public:
    void connectLine() override
    {
        // Боже, благослови того человека, который будет пытаться в этом разобраться
        // UPD: Боже, благослови меня это писавшего
        for(auto i = m_line.begin(); i != m_line.end(); i++)
        {
            // Кусок, который связывает связный список
            auto curPos = i, prevPos = i, nextPos = i;
            if (curPos != m_line.begin()) prevPos = i--;
            if (curPos != m_line.end()  ) nextPos = i++;
            if (nextPos!= m_line.end())  // Если справа не с чем связываться, то и не связываемся
            {
                (*curPos)->rightConnect(*nextPos,
                                     *(std::find_if(m_spansList.begin(),
                                                    m_spansList.end(),
                                                    [&](const Span &Sp) {
                                                        return (Sp.getLeft()  == (*curPos) ->getName() &&
                                                                Sp.getRight() == (*nextPos)->getName());
                                                    })));
            }
            if (i != m_line.begin())    // Если слева не с чем связываться, то и не связываемся
            {
                auto spanPos = std::find_if(m_spansList.begin(), m_spansList.end(),
                                            [&](const Span& Sp) { return (Sp.getLeft() == (*prevPos)->getName()
                                              && Sp.getRight() == (*curPos)->getName()) ;});
                (*curPos)->leftConnect(*prevPos, *spanPos);
            }
            // Искать вперед так можно, а вот назад придётся люто костылить
            // Внизу кусок, который связывает скип-лист
            for (int j = 0; j < (*i)->getSize(); j++)
            {
                auto r_pos = std::find_if(i, m_line.end(),
                                    [&]( Station *it) { return (it->getSize() == j);} );
                (*i)->rightLayerConnect(j, (*r_pos),
                                        calculateTravelTime_min((*i)->getNumber(), (*r_pos)->getNumber()),
                                        calculateTravelTime_max((*i)->getNumber(), (*r_pos)->getNumber()));
                // Поехали костыли
                auto pos_cur_st = std::find_if(m_line.rbegin(), m_line.rend(),
                                               [&]( Station *it) { return (it->getNumber() == (*i)->getNumber());});
                auto l_pos = std::find_if(pos_cur_st, m_line.rend(),
                                          [&] (Station *it) { return (it->getSize() == j);});
                (*i)->leftLayerConnect(j, (*l_pos),
                                       calculateTravelTime_min((*i)->getNumber(), (*l_pos)->getNumber()),
                                       calculateTravelTime_max((*i)->getNumber(), (*l_pos)->getNumber()));

            }
        }
        m_head = *m_line.begin();
        m_tail = *m_line.rbegin();

    }
    void print() { std::cout << "Hello world " << std::endl; }
};


#endif //RAILWAY_LINE_H
