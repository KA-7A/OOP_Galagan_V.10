//
// Created by ka_7a on 19.02.2021.
//

#include "line.h"

void Line::connectLine() // Супер наивный алгоритм. Наверняка, если хорошо подумать, то можно найти что-то нормальное
{
    std::sort(m_line.begin(), m_line.end(), [] (const Station* S1, const Station* S2) -> bool { return (*S1 < *S2); });
    std::vector<Span> Sp = m_spansList;
    std::vector<Station*> St = m_line;
    for (long unsigned int i = 0; i < St.size(); i++)
    {
        Station curSt = *St[i];
        std::string st_name = curSt.getName();
        for (int j = 0; j < Sp.size(); j++) {
            Span curSp = Sp[j];
            std::string left = curSp.getLeft(), right = curSp.getRight();
            if (left == st_name && left!=right)
                for (long unsigned int k = 0; k < St.size(); k++)
                    if (St[k]->getName() == right)
                    {
                        m_line[i]->rightConnect(m_line[k],m_spansList[j]);
                        break;
                    }

            if (right == st_name && left!=right)
                for (long unsigned int k = 0; k < St.size(); k++)
                    if (St[k]->getName() == left)
                    {
                        m_line[i]->leftConnect(m_line[k],m_spansList[j]);
                        break;
                    }
        }
    }
    assert(checkLine() == 2);   // Проверка на случай, если у нас не хватает слишком большого количества перегонов

    for (int i = 0; i < m_line.size(); i++) // Цикл не меняю на модную конструкцию, которую предлагает IDE, потому что к ней не привык
        if (m_line[i]->getLeftAddr() == nullptr) // Гарантированно получаем левый конец ветки
        {
            m_head = m_line[i];
            break;
        }
    for (int i = 0; i < m_line.size(); i++) // Вообще такой вот способ мне не очень нравится, тут вроде нужны итераторы
        if (m_line[i]->getRightAddr() == nullptr) // Гарантированно получаем правый конец ветки
        {
            m_tail = m_line[i];
            break;
        }
    //printLine();
}

double Line::calculateTravelTime_min(int n1, int n2) const {
    {
        /*
         * Смысл: проходим по всему двусвязному списку с начала и до конца, отмечаем...
         * Ну, понятно, что отмечаем и считаем
         */
        Station *head = m_head, *tail;
        while (head != nullptr && head->getNumber() != n1 && head->getNumber() != n2)
            head = head->getRightAddr();
        if (head == nullptr) {
            std::cout << "Station with that number did not found!" << std::endl;
            return -1;
        }
        double time_min = 0;
        if (head->getNumber() == n1) {
            tail = head;
            while (tail != nullptr && tail->getNumber() != n2) {
                time_min += tail->getRightSpan().getTime_min();
                tail = tail->getRightAddr();
            }
            if (tail == nullptr) {
                std::cout << "Station with that number did not found!" << std::endl;
                return -1;
            }
        } else {
            tail = head;
            while (tail != nullptr && tail->getNumber() != n1) {
                time_min += tail->getRightSpan().getTime_min();
                tail = tail->getRightAddr();
            }
            if (tail == nullptr) {
                std::cout << "Station with that number did not found!" << std::endl;
                return -1;
            }
        }
        return time_min;
    }
}

double Line::calculateTravelTime_max(int n1, int n2) const {
    Station *head = m_head, *tail;
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

