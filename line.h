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
#include <string>
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

#define SIZE 4096

using namespace rapidjson;

class Line {
protected:      // Делать protected -- нехорошо, но оно нужно только для дочернего класса
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
    Line() { m_head = m_tail = nullptr; }

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
    inline void setName  (std::string name) { m_name = std::move(name); }
    inline void setName  (const char* name) { m_name = name;            }
    inline void spanPushBack(const Span& s) { m_spansList.push_back(s); }

    void printFullAllStationsInfo_list()  const{
        Station *head = m_head;
        while (head != m_tail)
        {
            head ->printFullInfo();
            head = head->getRightAddr();
        }
        head->printFullInfo();
    }
    void printShortAllStationsInfo_list() const{
        Station *head = m_head;
        while (head != m_tail)
        {
            head ->printShortInfo();
            head = head->getRightAddr();
        }
        head->printShortInfo();
    }

    bool isNumFree(int number)
    {
        for (auto i = m_line.begin(); i != m_line.end(); i++)
        {
            if ((*i)->getNumber() == number)
                return false;
        }
        return true;
    }

    int get_ShortAllStationsInfo_list(char * c_msg, int size) const{
        std::string msg = "";
        Station *head = m_head;
        while (head != m_tail) {
            char tmp_msg[SIZE];
            if (!head->get_ShortInfo(tmp_msg, SIZE))
                msg += tmp_msg;
            else
                return -1;
            head = head->getRightAddr();
        }
        char tmp_msg [SIZE];
//        std::cout << msg;
        if(!head ->get_ShortInfo(tmp_msg, SIZE))
        {
            msg += tmp_msg;
            strncpy(c_msg, msg.c_str(), msg.size());
            return 0;
        }
        else {
            return -1;
        }
    }
    int get_FullAllStationsInfo_list (char * c_msg, int size) const{
        std::string msg = "";
        Station *head = m_head;
        while (head != m_tail)
        {
            char tmp_msg [SIZE];
            if(!head ->get_FullInfo(tmp_msg, SIZE))
                msg += tmp_msg;
            else
                return -1;
            head = head->getRightAddr();
        }
        char tmp_msg [SIZE];
        if(!head ->get_FullInfo(tmp_msg, SIZE)){
            msg += tmp_msg;
            strncpy(c_msg, msg.c_str(), msg.size());
            return 0;
        }
        else
            return -1;
    }
// ## Получаем на выход информацию о станции
    std::string getName() const   { return m_name;           }
    Station * getEndStation()     { return *m_line.rbegin(); }
// ## Метод связывания вектора в связный список
    virtual void connectLine();

// ## Методы с расчетом максимального/ минималььного
// РАБОТАЮТ С УЖЕ СВЯЗНЫМ СПИСКОМ
    double calculateTravelTime_min (int n1, int n2) const;
    double calculateTravelTime_max (int n1, int n2) const;
    Station* findStationByName(const std::string &name)  // Функция, которая возвращает станцию из списка по имени
    {
        for (Station *cur = m_head; cur != nullptr; cur = cur ->getRightAddr())
            if (cur->getName() == name) return cur;
        return nullptr;
    }
    Station* findStationByNumber(int number)
    {
        for (auto  i = m_line.begin(); i != m_line.end(); i++)
        {
            if ((*i)->getNumber() == number)
                return *i;
        }
        return nullptr;
    }

    virtual ~Line() = default;

};

class S_Line: public Line
{
public:
    void connectLine() override
    {
        // Боже, благослови того человека, который будет пытаться в этом разобраться
        // UPD: Боже, благослови меня это писавшего
        // UPD2:Крч я забил писать новый код и поэтому просто скопировал старый кусок, который отвечал за связывание
        // линии в двусвязный список. Получилось вроде норм, надо придумать, как это сделать красивее
        for (auto cur_Station = m_line.begin(); cur_Station != m_line.end(); cur_Station++)   // Каждую станцию надо связать
        {
            const std::string& st_name = (*cur_Station)->getName();                           // Получаем имя станции (в той же цели)
            for (auto cur_Span = m_spansList.begin(); cur_Span != m_spansList.end(); cur_Span++) { // Проходим по вектору перегонов
                std::string left =  (*cur_Span).getLeft();
                std::string right = cur_Span->getRight(); // Получим имена правой и левой станции перегона
                if (left == st_name && left!=right)                             // Если имя станции совпадает с левым краем
                    for (auto test_Station = m_line.begin(); test_Station != m_line.end(); test_Station++)       // Ищем правый конец этого перегона (не знаем заведомо, где он)
                        if ((*test_Station)->getName() == right)                           // Нашли?
                        {
                            (*cur_Station)->rightConnect(*test_Station, *cur_Span);   // Соединяем это дело
                            break;
                        }
                if (right == st_name && left!=right)                                       // Аналогично в обратную сторону
                    for (auto test_Station = m_line.begin(); test_Station != m_line.end(); test_Station++)
                        if ((*test_Station)->getName() == left)
                        {
                            (*cur_Station)->leftConnect(*test_Station, *cur_Span);
                            break;
                        }
            }
        }
        for(auto i = m_line.begin(); i != m_line.end(); i++)
        {
            /*
            // Кусок, который связывает связный список
            auto curPos = i, prevPos = i, nextPos = i;
            if (curPos != m_line.begin()) prevPos = --i;
            if (curPos != m_line.end()  ) nextPos = ++i;
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
            */
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
        m_head = *(m_line.begin() );
        m_tail = *(m_line.rbegin());
        std::cout << "Line \""<< this->getName() <<"\" connected!" << std::endl;
    }
};



#endif //RAILWAY_LINE_H
