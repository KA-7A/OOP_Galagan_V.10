//
// Created by ka_7a on 19.02.2021.
//

#include "line.h"

void Line::connectLine() // Супер наивный алгоритм. Наверняка, если хорошо подумать, то можно найти что-то нормальное
{
    //std::sort(m_line.begin(), m_line.end(), [] (const Station* S1, const Station* S2) -> bool { return (*S1 < *S2); });
    std::vector<Span> Sp = m_spansList;
    for (auto cur_Station = m_line.begin(); cur_Station != m_line.end(); cur_Station++)   // Каждую станцию надо связать
    {
        const std::string& st_name = (*cur_Station)->getName();                       // Получаем имя станции (в той же цели)
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
    assert(checkLine() == 2);   // Проверка на случай, если у нас не хватает слишком большого количества перегонов

    // Определяем голову и хвост линии
    for (unsigned int i = 0; i < m_line.size(); i++) // Цикл не меняю на модную конструкцию, которую предлагает IDE, потому что к ней не привык
        if (m_line[i]->getLeftAddr() == nullptr)     // Гарантированно получаем левый конец ветки
        {
            m_head = m_line[i];
            break;
        }
    for (unsigned int i = 0; i < m_line.size(); i++) // Вообще такой вот способ мне не очень нравится, тут вроде нужны итераторы
        if (m_line[i]->getRightAddr() == nullptr)    // Гарантированно получаем правый конец ветки
        {
            m_tail = m_line[i];
            break;
        }
    //printLine();
}

double Line::calculateTravelTime_min(int n1, int n2) const {
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

