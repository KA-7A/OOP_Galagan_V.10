/*
 * Обычная станция: описывается порядковым номером на линии, именем и средним грузопотоком. Должна поддерживать
 * регистрацию изменения грузопотока и сравнение по порядковому номеру с помощью операций < и ==.
 * Created by ka_7a on 19.02.2021.
*/


#ifndef RAILWAY_STATION_H
#define RAILWAY_STATION_H
#include <string>
#include <utility>
#include "span.h"

class Station{
private:
    int m_number;               // Номер станции, имеет вид (№ветки * 1000 + №станции)
    int m_av_traffic;           // Значение среднего траффика
    std::string m_name;         // Имя станции
    Station *m_next, *m_prev;   // Указатели на соседние станции. Ну потому что по ТЗ это д.б. связанный список. Если что-то нулл, то это конечная
    Span m_left, m_right;       // Указатели на перегоны до соседних станций.
    bool m_isRegular;           // true -- станция обычная, false -- пересадочная. Мне не нрав такой момент, но пусть будет
public:
    Station(void)
    {
        m_number     = 0;
        m_av_traffic = 0;
        m_name = "Unnamed\n";
        m_next = m_prev = nullptr;
        m_isRegular  = true;     // По дефолту все станции у нас сначала обычные
    }
    Station(int number, int av_traffic, std::string name) {  // Такой вот странный конструктор, который, почему-то, не вызывается. Сцука такая
        m_number = number;
        m_av_traffic = av_traffic;
        m_name = name;
        m_isRegular = true;
        m_next = nullptr;
        m_prev = nullptr;
    }
// #### Производим манипуляции с самой станцией #### //
    void fill(int number, int av_traffic, std::string name){    // Заполняем основные поля станции
        m_number = number;
        m_name = std::move(name);
        m_av_traffic = av_traffic;
        m_next = m_prev = nullptr;
        m_isRegular = true;
    }
    // ## Соединяем станции ## //
    inline void rightConnect(Station *next, Span &right )  { m_next = next ; m_right = right; }
    inline void leftConnect (Station *prev, Span &left)    { m_prev = prev ; m_left  = left;  }

    // ## Меняем кое-какие показатели ## //
    inline void changeToType (bool type)        { m_isRegular = type;         }
    inline void changeTraffic(int new_traffic)  { m_av_traffic = new_traffic; }

    virtual // ## Выдаем всю информацию о станции на печать ## //
    void printFullInfo()  const {
        std::cout << "_________________________" << std::endl;
        std::cout << "| name: " << m_name             << std::endl <<
                     "| num : " << m_number           << std::endl <<
                     "| av_traffic: " << m_av_traffic << std::endl <<
                     "| Type: regular"                << std::endl <<
                     "| neighbours: "   << std::endl;
        if (getLeftAddr() ) std::cout << "| l:" << getLeftName()  << std::endl;
        if (getRightAddr()) std::cout << "| r:" << getRightName() << std::endl;
        std::cout  << "|________________________"  << std::endl;
    }
    void printShortInfo() const {
        std::cout << "|---------------------------" << std::endl;
        std::cout << "| name: " << m_name << std::endl <<
                     "| num : " << m_number << std:: endl; //"_____________" << std::endl;
    }

// #### Получаем информацию о самой станции #### //
    inline int  getNumber()      const { return m_number;       }
    inline int  getTraffic()     const { return m_av_traffic;   }
    inline std::string getName() const { return m_name;         }
    inline bool isCrossing()     const { return (!m_isRegular); }

// #### Получаем информацию о соседях станции ### //
    inline std::string getLeftName()   const { return m_prev->getName(); }
    inline std::string getRightName()  const { return m_next->getName(); }
    inline Station * getLeftAddr()     const { return m_prev; }
    inline Station * getRightAddr()    const { return m_next; }
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

    ~Station()
    {
        std::cout << "Pshel nah" << std::endl;
    }
};
#endif //RAILWAY_STATION_H
