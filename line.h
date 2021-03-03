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

    void sortLine() {   // Этот метод в private, потому что если отсортировать вектор, а потом не связать всё заново, то ничего не будет работать
        for (int i = 0; i < m_line.size()-1; i++)
        {
            int min_index = i;
            for (int j = i+1; j < m_line.size(); j++)
                if (m_line[j]<m_line[min_index])
                    min_index = j;
            if (min_index != i) {
                Station tmp;
                tmp = m_line[i];
                m_line[i] = m_line[min_index];
                m_line[min_index] = tmp;
            }
        }
    }
public:
    Line() {
        m_head = m_tail = nullptr;
    }
// ## Под замену ## //
    inline void addStationToLine(const Station& S)        { m_line.push_back(S); }
    inline void addStationToLine(const CrossingStation& S){ m_line.push_back(S); }
// ## Меняем кое-что внутри нашей ветки
    inline void writeName(std::string name) { m_name = std::move(name); }
    inline void spanPushBack(const Span& s) { m_spansList.push_back(s); }
// ## Получаем на выход информацию о станции
    std::string getName() const{ return m_name; }
    void printFullAllStationsInfo_list()  const{
        Station *head = m_head, *tail = m_tail;
        while (head != nullptr)
        {
            head ->printFullInfo();
            head = head->getRightAddr();
        }
    }
    void printShortAllStationsInfo_list() const{
        Station *head = m_head, *tail = m_tail;
        while (head != nullptr)
        {
            head ->printShortInfo();
            head = head->getRightAddr();
        }
    }

    void connectLine() {
        sortLine();
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
        assert(checkLine() == 2);
    }

    int checkLine() const {
        int counter = 0;
        for (int i = 0; i < m_line.size(); i++)
            if (m_line[i].getRightAddr() == nullptr || m_line[i].getLeftAddr() == nullptr)
                counter++;
        return counter;
    }

    double calculateTravelTime_min (int n1, int n2) const {
        /*
         * Тут получаем на вход номера станций и считаем расстояние между ними :)
         */
        Station *head = m_head, *tail = m_tail;
        while (head != nullptr && head->getNumber() != n1 && head->getNumber() != n2)
            head = head->getRightAddr();
        if (head == nullptr)
            {
                std::cout << "Station with that number did not found!" << std::endl;
                return -1;
            }
        double time_min = 0;
        if (head->getNumber() == n1 )
        {
            tail = head;
            while (tail != nullptr && tail->getNumber() != n2 ){
                time_min += tail->getRightSpan().getTime_min();
                tail = tail->getRightAddr();
            }
            if (tail == nullptr)
            {
                std::cout << "Station with that number did not found!" << std::endl;
                return -1;
            }
        }
        else
        {
            tail = head;
            while (tail != nullptr && tail->getNumber() != n1) {
                time_min += tail->getRightSpan().getTime_min();
                tail = tail->getRightAddr();
            }
            if (tail == nullptr)
            {
                std::cout << "Station with that number did not found!" << std::endl;
                return -1;
            }
        }
        return time_min;

    }
    double calculateTravelTime_max (int n1, int n2) const {
        Station *head = m_head, *tail = m_tail;
        while (head != nullptr && head->getNumber() != n1 && head->getNumber() != n2)
            head = head->getRightAddr();
        if (head == nullptr)
        {
            std::cout << "Station with that number did not found!" << std::endl;
            return -1;
        }
        double time_max = 0;
        if (head->getNumber() == n1)
        {
            tail = head;
            while (tail != nullptr && tail->getNumber() != n2 ){
                time_max += tail->getRightSpan().getTime_max();
                tail = tail->getRightAddr();
            }
            if (tail == nullptr)
            {
                std::cout << "Station with that number did not found!" << std::endl;
                return -1;
            }
        }
        else
        {
            tail = head;
            while (tail != nullptr && tail->getNumber() != n1) {
                time_max += tail->getRightSpan().getTime_max();
                tail = tail->getRightAddr();
            }
            if (tail == nullptr)
            {
                std::cout << "Station with that number did not found!" << std::endl;
                return -1;
            }
        }
        return time_max;
    }

};


#endif //RAILWAY_LINE_H
