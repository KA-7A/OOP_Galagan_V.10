/*
 * Обычная станция: описывается порядковым номером на линии, именем и средним грузопотоком. Должна поддерживать
 * регистрацию изменения грузопотока и сравнение по порядковому номеру с помощью операций < и ==.
 * Created by ka_7a on 19.02.2021.
*/


#ifndef RAILWAY_STATION_H
#define RAILWAY_STATION_H
#include <string>
#include <utility>
#include <iostream>
#include "span.h"

class Span;

class Station{
private:
    int m_number;               // Номер станции, имеет вид (№ветки * 1000 + №станции)
    int m_av_traffic;           // Значение среднего траффика
    std::string m_name;         // Имя станции
    Station *m_next, *m_prev;   // Указатели на соседние станции. Ну потому что по ТЗ это д.б. связанный список. Если что-то нулл, то это конечная
    Span m_left, m_right;       // Перегоны до соседних станций. Надо использовать указатели. Исправить.
public:
    Station()   // Дефолтный конструктор
    {
        m_number     = 0;
        m_av_traffic = 0;
        m_name = "Unnamed";
        m_next = m_prev = nullptr;
    }
    Station(int number, int av_traffic, std::string name) {
        m_number = number;
        m_av_traffic = av_traffic;
        m_name = name;
        m_next = nullptr;
        m_prev = nullptr;
    }
// #### Производим манипуляции с самой станцией #### //
    void fill(int number, int av_traffic, std::string name){    // Заполняем основные поля станции

        m_number = number;
        m_name = std::move(name);
        m_av_traffic = av_traffic;
    }
    // ## Соединяем станции ## //
    inline void rightConnect(Station *next, Span &right )  { m_next = next ; m_right = right; }
    inline void leftConnect (Station *prev, Span &left)    { m_prev = prev ; m_left  = left;  }

    // ## Меняем кое-какие показатели ## //
    inline void setTraffic   (int traffic)      { m_av_traffic = traffic; }
    inline void setName      (std::string name) { m_name   = name;        }
    inline void setNumber    (int number)       { m_number = number;      }

    // ## Выдаем всю информацию о станции на печать ## //
    virtual void printFullInfo()  const {
        std::cout << "_________________________" << std::endl;
        std::cout << "| name: "         << m_name       << std::endl <<
                     "| num : "         << m_number     << std::endl <<
                     "| av_traffic: "   << m_av_traffic << std::endl <<
                     "| Type: regular"                  << std::endl <<
                     "| neighbours: "                   << std::endl;
        if (getLeftAddr() ) std::cout << "| l:" << getLeftName()  << std::endl;
        if (getRightAddr()) std::cout << "| r:" << getRightName() << std::endl;
        std::cout  << "|________________________"  << std::endl;
    }
    void printShortInfo() const {
        std::cout << "|---------------------------" << std::endl;
        std::cout << "| name: " << m_name   << std::endl <<
                     "| num : " << m_number << std::endl;
    }

// #### Получаем информацию о самой станции #### //
    inline int getNumber()              const { return m_number;       }
    inline int getTraffic()             const { return m_av_traffic;   }
    inline const std::string& getName() const { return m_name;         }
    inline virtual bool isCrossing()    const { return false;          }

// #### Получаем информацию о соседях станции ### //
    inline const std::string getLeftName()   const { if (m_prev!= nullptr) return m_prev->getName(); else return nullptr; }
    inline const std::string getRightName()  const { if (m_next!= nullptr) return m_next->getName(); else return nullptr; }
    /*
     * Убрали тут возвращение по ссылке потому что компилятор ругается, что в случае else у нас возвращается
     * временный объект. И он прав, оставлю пока вот так.
     */
    inline Station * getLeftAddr()     const { if (m_prev!= nullptr) return m_prev; else return nullptr; }
    inline Station * getRightAddr()    const { if (m_next!= nullptr) return m_next; else return nullptr; }
    inline double getTimeToLeft_min()  const { return m_left.getTime_min() ; }
    inline double getTimeToLeft_max()  const { return m_left.getTime_max() ; }
    inline double getTimeToRight_min() const { return m_right.getTime_min(); }
    inline double getTimeToRight_max() const { return m_right.getTime_max(); }
// И перегонах
    inline Span getLeftSpan()  const { return m_left;  }
    inline Span getRightSpan() const { return m_right; }
// #### Перегружаем операторы ##### //
    bool operator== (const Station &S2) const { return (m_number == S2.m_number); }
    bool operator<  (const Station &S2) const { return (m_number < S2.m_number) ; }

    virtual ~Station(){}
};
#endif //RAILWAY_STATION_H
