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
        m_minTime = m_maxTime = 0;
    }
    Span(const char* S1, const char* S2, double minTime, double maxTime){ // Передали по ссылке, теперь можем их менять как вздумается

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
    Span(const char* S1, const char* S2, double time)
    {
        if (time == 0) {
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
// ## Получем имена соседних станций
    inline std::string getLeft()  const { return m_left_st; }
    inline std::string getRight() const { return m_right_st; }
// ## Получаем время, за которое можно пересечь этот перегон
    inline double getTime_min() const   { return m_minTime; }
    inline double getTime_max() const   { return m_maxTime; }
// ## Меняем время перегона. Больше ничего менять не планируется
    inline void setTime_min(double min) { m_minTime = min; }
    inline void setTime_max(double max) { m_maxTime = max; }
// ## Деструктор
    ~Span()
    {
        m_left_st.clear();
        m_right_st.clear();
    }
};


#endif //RAILWAY_SPAN_H
