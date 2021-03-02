/*
 * Перегон: описывается прилегающими к нему станциями и парой значений времени (измеряемого в часах), которые
 * показывают минимально допустимое и максимально допустимое время прохождения перегона.
 * Created by ka_7a on 19.02.2021.
*/

#ifndef RAILWAY_SPAN_H
#define RAILWAY_SPAN_H
#include "station.h"
#include <cassert>

class Span {
private:
    std::string m_left_st, m_right_st;
    double m_minTime, m_maxTime;
public:
    Span() {
        m_minTime = 0.9;
        m_maxTime = 1.1;
    }
    Span(std::string &S1, std::string &S2, double minTime, double maxTime) // Передали по ссылке, теперь можем их менять как вздумается
    {
        if (minTime > maxTime) {
            std::cout << "Incorrect input: minTime > maxTime! I'll swap it" << std::endl;
            m_minTime = maxTime;
            m_maxTime = minTime;
        } else {
            m_minTime = minTime;
            m_maxTime = maxTime;
        }
        m_left_st = S1;
        m_right_st = S2;
    }
    Span(const char* S1, const char * S2, double time) // Передали по ссылке, теперь можем их менять как вздумается
    {
        if (!time) {
            m_maxTime = m_minTime = 0;
        }
        else
        {
            m_minTime = time;
            m_maxTime = m_minTime + 0.5;
        }
        m_left_st = S1;
        m_right_st = S2;
    }
    inline std::string getLeft() { return m_left_st; }
    inline std::string getRight() { return m_right_st; }
};


#endif //RAILWAY_SPAN_H
