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
    void printAllStationsInfo_list() const{
        Station *head = m_head, *tail = m_tail;
        while (head!=tail)
        {
            head ->printInfo();
            head = head->getRightAddr();
        }
        tail->printInfo();
    }

    void connectLine() {
        std::vector<Span> Sp = m_spansList;
        std::vector<Station> St = m_line;
        for (long unsigned int i = 0; i < St.size(); i++)
        {
            Station curSt = St[i];
            std::string st_name = curSt.getName();
            for (int j = 0; j < Sp.size(); j++) {
                Span curSp = Sp[j];
                std::string left = curSp.getLeft(), right = curSp.getRight();
                if (left == st_name && left!=right)
                    for (long unsigned int k = 0; k < St.size(); k++)
                        if (St[k].getName() == right)
                        {
                            m_line[i].rightConnect(&m_line[k],m_spansList[j]);
                            //std::cout <<  st_name << " -> " << right  << std::endl;
                            break;
                        }

                if (right == st_name && left!=right)
                    for (long unsigned int k = 0; k < St.size(); k++)
                        if (St[k].getName() == left)
                        {
                            m_line[i].leftConnect(&m_line[k],m_spansList[j]);
                            //std::cout << left << " <- " << st_name << std::endl;
                            break;
                        }
            }
        }
        for (int i = 0; i < m_line.size(); i++)
            if (m_line[i].getLeftAddr() == nullptr) // Гарантированно получаем левый конец ветки
            {
                m_head = &m_line[i];
                break;
            }
        for (int i = 0; i < m_line.size(); i++)
            if (m_line[i].getRightAddr() == nullptr) // Гарантированно получаем левый конец ветки
            {
                m_tail = &m_line[i];
                break;
            }
        //printLine();
    }
    void printLine() const {
        std::cout << "##################################" << std::endl;
        Station * cur = m_head;
        while(cur != nullptr )
        {
            cur->printInfo();
            cur = cur ->getRightAddr();
        }
        std::cout << "##################################" << std::endl;
    }

    double calculateTravelTime_min (int n1, int n2) const {
        /*
         * Тут получаем на вход номера станций и считаем расстояние между ними :)
         */
        Station *head = m_head, *tail = m_tail;
        while (head->getNumber() != n1 && head->getNumber() != n2)
            head = head->getRightAddr();
        double time_min = 0;
        if (head->getNumber() == n1 )
        {
            tail = head;
            while (tail->getNumber() != n2 && tail != nullptr ){
                time_min += tail->getRightSpan().getTime_min();
                tail = tail->getRightAddr();

            }

        }
        else
        {
            tail = head;
            while (tail->getNumber() != n1 && tail != nullptr) {
                time_min += tail->getRightSpan().getTime_min();
                tail = tail->getRightAddr();
            }
        }
        return time_min;

    }
    double calculateTravelTime_max (int n1, int n2) const {
        Station *head = m_head, *tail = m_tail;
        while (head->getNumber() != n1 && head->getNumber() != n2)
            head = head->getRightAddr();
        double time_max = 0;
        if (head->getNumber() == n1)
        {
            tail = head;
            while (tail->getNumber() != n2){
                time_max += tail->getRightSpan().getTime_max();
                tail = tail->getRightAddr();

            }

        }
        else
        {
            tail = head;
            while (tail->getNumber() != n1) {
                time_max += tail->getRightSpan().getTime_max();
                tail = tail->getRightAddr();
            }
        }
        return time_max;

    }

};


#endif //RAILWAY_LINE_H
