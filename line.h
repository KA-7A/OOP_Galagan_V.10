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

using namespace rapidjson;

class Line {
private:
    std::vector<Station> m_line;    // Собсна, вектор, в котором будет храниться наш двусвязный список (так же, чтобы он нигде не потерялся)
    std::vector<Span> m_spansList;  // Сделаем такую вот штуку, чтобы гарантированно сохранить все перегоны (чтобы ничто нигде не затерлось)
    Station *m_head, *m_tail;        // Указатели на начало и конец ветки..
    std::string m_name;
public:
    Line() {
        m_head = m_tail = nullptr;
    }
// ## Под замену ## //
    inline void addStationToLine(const Station& S) { m_line.push_back(S); }
    inline void addStationToLine(const CrossingStation& S){ m_line.push_back(S); }
    inline void writeName(std::string name) { m_name = std::move(name); }
    inline void spanPushBack(const Span& s) { m_spansList.push_back(s); }
    std::string getName() const{ return m_name; }

    void printAllStationsInfo() const {
        std::cout << "##### Line " << m_name << " #####" << std::endl;
        for (const auto & i : m_line) {
            i.printInfo();
            if (i.isCrossing())
                std::cout << "Crossing station, yess" << std::endl;
                //m_line[i].printCrossStations();
        }

    }

    void connectLine() {
        std::vector<Span> Sp = m_spansList;
        std::vector<Station> St = m_line;
        for (long unsigned int i = 0; i < St.size(); i++)
        {
            Station curSt = St[i];
            std::string st_name = curSt.getName();
            for (auto & j : Sp) {
                Span curSp = j;
                std::string left = curSp.getLeft(), right = curSp.getRight();
                if (left == st_name)
                    for (long unsigned int k = 0; k < St.size(); k++)
                        if (St[k].getName() == right)
                        {
                            St[i].rightConnect(&St[k], j);
                            //std::cout <<  st_name << " -> " << right  << std::endl;
                            break;
                        }

                if (right == st_name)
                    for (long unsigned int k = 0; k < St.size(); k++)
                        if (St[k].getName() == left)
                        {
                            St[i].leftConnect(&St[k], j);
                            //std::cout << left << " <- " << st_name << std::endl;
                            break;
                        }
            }
        }
        m_line = St;
        m_spansList = Sp;
        for (auto & i : m_line)
            if (i.getLeftName() == i.getName()) // Гарантированно получаем левый конец ветки
            {
                m_head = &i;
                break;
            }
        for (auto & i : m_line)
            if (i.getRightName() == i.getName()) // Гарантированно получаем правый конец ветки
            {
                m_tail = &i;
                break;
            }
        printLine();
    }
    void printLine() const {
        std::cout << "##################################" << std::endl;
        Station * cur = m_head;
        while(cur->getRightName() != cur->getName())
        {
            cur->printInfo();
            cur = cur ->getRightAddr();
        }
        cur->printInfo();
        std::cout << "##################################" << std::endl;
    }

};


#endif //RAILWAY_LINE_H
